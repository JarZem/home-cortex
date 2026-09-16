# Home Cortex – principy a cíle

> Tento dokument definuje základní poslání a hranice systému. Podrobné definice použitých pojmů budou vedeny v `Glossary`.

## Poslání Home Cortex

[FIXED] **Home Cortex je otevřený systém pro průběžné vytváření a vyhodnocování modelu domu a jeho okolí. Jeho hlavním účelem je určit, v jakém stavu se dům nachází, jak se do tohoto stavu dostal, s jakou mírou jistoty je tento stav znám a co z něj vyplývá pro další vývoj a možné reakce systému.**

[FIXED] **Home Cortex získává informace z dostupných interních i externích zdrojů, vyhodnocuje jejich vzájemné vztahy a historii a podle aktuálního `Context` může reagovat prostřednictvím dostupných `Actuator`, změnou způsobu získávání dalších informací nebo interakcí s uživatelem.**

[FIXED] **Systém není omezen na automatizaci zařízení. Jeho úkolem je také rozpoznávat stavy, rizika, potřeby a události, na které nemůže reagovat přímo, a v takových případech vhodným způsobem zapojit člověka.**

## World Model a World State

[FIXED] Home Cortex se nesnaží udržovat pouze stav vlastního software. Vytváří `World Model` – model relevantní části reálného světa spojeného s domem.

Aktuální vyhodnocený stav tohoto modelu označujeme jako `World State`.

`World State` může zahrnovat například stav místností, zařízení, osob, prostředí, energií, vody, počasí, známých rizik, probíhajících činností a dalších skutečností relevantních pro dům.

[FIXED] `World State` nemusí být znám s absolutní jistotou. Odvozené skutečnosti musí být možné reprezentovat spolu s jejich `Confidence`, původem a podpůrnými `Evidence`.

## Historie je součástí porozumění

[FIXED] Pro Home Cortex není důležitý pouze současný stav, ale také způsob, jakým tento stav vznikl.

Například informace `window.bedroom = CLOSED` sama nemusí dostatečně popisovat situaci. Relevantní může být posloupnost událostí, která jí předcházela: zjištěné riziko přicházející bouřky, upozornění člověka, jeho přesun mezi místnostmi a následné fyzické zavření okna.

Historie proto není pouze diagnostický log. Je zdrojem `Evidence`, vysvětlení současného `World State` a budoucích trénovacích dat.

[TODO-DESIGN] Přesný model historie, neměnných `Event`, odvozených stavů a možnosti zpětného přehrání (`Replay`) bude definován samostatně.

## Zdroje informací nejsou pouze fyzické senzory

[FIXED] Home Cortex nesmí chápat svět pouze prostřednictvím fyzických `Sensor` instalovaných v domě.

`Source` může poskytovat `Observation` o světě bez ohledu na to, zda jde o fyzické čidlo, jiný informační systém nebo externí službu.

Příklady zahrnují fyzické senzory, Home Assistant, Zigbee zařízení, síťovou infrastrukturu, telefon, čas, kalendářní informace, předpověď počasí, radar srážek nebo službu detekce blesků.

[FIXED] Home Cortex musí být schopen kombinovat více různých `Source` a posuzovat jejich informace v kontextu, včetně jejich nejistoty, stáří a vzájemné podpory nebo rozporu.

## Reakce nemusí být pouze automatická

[FIXED] Výsledkem `Decision` nemusí být pouze automatická `Action` provedená akčním prvkem.

Pokud systém nemá vhodný `Actuator`, může být vykonavatelem člověk. Home Cortex musí být schopen vytvořit požadavek, upozornění, `Task` nebo `Risk`, předat jej vhodnému člověku a následně sledovat, zda byl problém skutečně vyřešen.

Potvrzení člověka a skutečné vyřešení problému jsou dvě rozdílné skutečnosti.

## Aktivní získávání informací

[FIXED] Home Cortex není pouze pasivním příjemcem informací. Pokud je pro rozhodnutí potřeba další informace a existuje způsob, jak ji získat, může systém aktivně změnit způsob pozorování světa.

To může například znamenat dočasné zapnutí energeticky náročnějšího `Sensor`, změnu frekvence měření nebo vyžádání další informace z dostupného `Source`.

[TODO-DESIGN] Způsob rozhodování o hodnotě informace, energetické ceně měření a aktivním řízení senzorů bude definován v samostatné části.

## Oblast působnosti

[FIXED] Home Cortex je orientován na dům, jeho provoz, relevantní okolí a osoby ve vztahu k domu. Není jeho základním cílem stát se obecným osobním asistentem pro oblasti nesouvisející s domem.

Do oblasti Home Cortex však přirozeně mohou patřit například:

- spotřeba a dostupnost elektřiny, vody a dalších energií,
- výroba a ukládání energie,
- technické systémy domu,
- bezpečnost a provozní rizika,
- údržba, servis, životnost a revize,
- stav prostředí uvnitř domu,
- počasí a další vnější podmínky relevantní pro dům,
- přítomnost, poloha a činnost osob, pokud jsou relevantní pro chování domu.

[PROVISIONAL] Přesná hranice mezi „relevantní pro dům“ a obecnou funkcí osobního asistenta bude zpřesňována podle konkrétních případů. Rozšiřování systému však nesmí rozmazat jeho primární účel.

## Učení

[FIXED] Běžné používání domu má být potenciálním zdrojem dalších `Evidence` a trénovacích dat.

Následný výsledek může zpětně zvýšit nebo snížit důvěru v předchozí odhady. Například fyzické zavření okna po požadavku systému může pomoci zpětně vyhodnotit předchozí odhad pohybu osoby přes několik prostorů domu a současně poskytnout data pro budoucí rozpoznávání podobných situací.

[FIXED] Home Cortex musí rozlišovat mezi tím, co bylo přímo pozorováno, co bylo v daném okamžiku odhadnuto a co bylo odvozeno až později z následných událostí.

[TODO-DESIGN] Konkrétní metody učení, aktualizace pravděpodobností a vytváření trénovacích dat budou specifikovány samostatně.

## Vysvětlitelnost

[FIXED] U významných odvozených stavů a rozhodnutí musí být možné zpětně zjistit, z jakých informací vznikly a proč Home Cortex jednal určitým způsobem.

Systém tedy nemá pouze odpovědět „co si myslím“, ale v přiměřené míře také „proč si to myslím“ a „proč jsem provedl tuto reakci“.

[TODO-DESIGN] Úroveň uchovávaného vysvětlení, auditní historie a její vztah k učení a `Replay` bude definována později.
