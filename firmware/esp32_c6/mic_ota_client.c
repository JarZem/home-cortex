#include "mic_ota_client.h"
#define PAGE 64u
#define MAX_PAGES 55u
static uint8_t crc8(const uint8_t *p,size_t n){
 uint8_t c=0;while(n--){c^=*p++;for(unsigned i=0;i<8;i++)c=(uint8_t)((c<<1)^((c&128u)?7u:0u));}return c;
}
static uint32_t image_crc(const uint8_t *p,size_t n){
 uint32_t c=~0u;size_t padded=(n+63u)&~63u;
 for(size_t j=0;j<padded;j++){
  c^=(j<n?p[j]:255u);
  for(unsigned k=0;k<8;k++)c=(c>>1)^((c&1u)?0xedb88320u:0u);
 }
 return ~c;
}
static int send_bytes(const mic_ota_bus_t *b,const uint8_t *p,size_t n){return b->send(b->ctx,p,n)==(int)n;}
static int read_bytes(const mic_ota_bus_t *b,uint8_t *p,size_t n,uint32_t ms){return b->recv(b->ctx,p,n,ms)==(int)n;}
mic_ota_result_t mic_ota_upload(const mic_ota_bus_t *b,
 const uint8_t *a,size_t na,const uint8_t *z,size_t nz,uint8_t *target){
 if(!b||!b->send||!b->recv||!a||!z)return MIC_OTA_IMAGE;
 uint8_t request[2]={0x42,0};request[1]=crc8(request,1);
 uint8_t ready[3];
 if(!send_bytes(b,request,2)||!read_bytes(b,ready,3,500))return MIC_OTA_IO;
 if(ready[0]!=0x81||ready[1]>1||ready[2]!=crc8(ready,2))return MIC_OTA_PROTOCOL;
 uint8_t dst=(uint8_t)(ready[1]^1u);
 const uint8_t *image=dst?z:a;size_t size=dst?nz:na;
 if(!size||size>MAX_PAGES*PAGE)return MIC_OTA_IMAGE;
 if(target)*target=dst;
 uint16_t count=(uint16_t)((size+63u)/64u);
 uint32_t crc=image_crc(image,size);
 uint8_t start[8]={0x40,(uint8_t)count,(uint8_t)(count>>8),
 (uint8_t)crc,(uint8_t)(crc>>8),(uint8_t)(crc>>16),(uint8_t)(crc>>24),0};
 start[7]=crc8(start,7);
 uint8_t ack;
 if(!send_bytes(b,start,8)||!read_bytes(b,&ack,1,500))return MIC_OTA_IO;
 if(ack!=0x7e)return MIC_OTA_PROTOCOL;
 uint8_t frame[67];
 for(uint16_t i=0;i<count;i++){
  frame[0]=0x41;frame[1]=(uint8_t)i;
  for(unsigned j=0;j<64;j++){size_t off=(size_t)i*64u+j;frame[2+j]=off<size?image[off]:255u;}
  frame[66]=crc8(frame,66);
  for(unsigned retry=0;retry<4;retry++){
   if(!send_bytes(b,frame,67))return MIC_OTA_IO;
   if(i+1u==count){
    /* MIC verifies CRC32, commits and resets without final ACK.
     * A BOOT frame [0x80,status,CRC8] is the success indication. */
    uint8_t boot[3];
    if(!read_bytes(b,boot,3,1500))return MIC_OTA_IO;
    if(boot[0]==0x80&&boot[2]==crc8(boot,2))return MIC_OTA_OK;
    return MIC_OTA_PROTOCOL;
   }
   if(!read_bytes(b,&ack,1,1000))return MIC_OTA_IO;
   if(ack==0x7e)break;
   if(ack!=0x7f)return MIC_OTA_PROTOCOL;
   if(retry==3)return MIC_OTA_RETRY_EXHAUSTED;
  }
 }
 return MIC_OTA_PROTOCOL;
}
