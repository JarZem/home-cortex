# Home Cortex – zařízení, identita a integrace

> Tento dokument rozvíjí základní principy `World Model` pro fyzická zařízení a spotřebiče. Neurčuje konkrétní implementační protokol ani konkrétní značky zařízení.

## Identita zařízení nemusí pocházet z přímé komunikace

[FIXED] **Home Cortex nesmí předpokládat, že identita, provozní stav nebo činnost zařízení musí být získány přímou komunikací se zařízením. Musí umožňovat jejich odvozování z charakteristického chování zařízení v čase a z kombinace více `Evidence`.**

Zařízení může být známo explicitně, pouze částečně, nebo může být jeho identita pouze hypotézou s určitou `Confidence`. Neznámá nebo nejistá identita nesmí znemožnit použití ostatních známých vlastností zařízení při vyhodnocování `World State`, `Risk`, `Expectation` nebo `Decision`.

Příkladem je elektrický spotřebič, který s Home Cortex přímo nekomunikuje. Jeho pravděpodobnou identitu a provozní stav může být možné odvozovat například z průběhu elektrického příkonu, charakteristických ON/OFF cyklů, změn příkonu, umístění zásuvky, přítomnosti a pohybu osob, spotřeby vody, vibrací, teplotních změn a historie předchozích pozorování.

[PROVISIONAL] Obecný pojem `Behavioral Signature` bude označovat rozpoznatelný vzorec chování objektu nebo zařízení v čase. `Power Signature` bude jeho specializací pro elektrické veličiny a jejich časový průběh.

[FIXED] `Behavioral Signature` ani `Power Signature` nejsou samy o sobě identitou zařízení. Jsou `Evidence`, která mohou podporovat nebo oslabovat jednu či více hypotéz o identitě, stavu nebo činnosti zařízení.

## Přímá komunikace se zařízeními

[FIXED] **Home Cortex musí být připraven využívat přímou nebo zprostředkovanou komunikaci se zařízením, pokud ji zařízení poskytuje, ale doménový model Home Cortex nesmí být závislý na jednom výrobci, jedné mobilní aplikaci, jednom komunikačním protokolu ani jednom integračním mechanismu.**

Moderní spotřebiče mohou poskytovat vlastní Wi-Fi komunikaci, lokální nebo cloudové API, mobilní aplikaci, proprietární protokol nebo integraci přes jiný systém. Typickými příklady mohou být pračka, sušička, kávovar nebo robotický vysavač. Jiný spotřebič nemusí poskytovat žádné datové rozhraní a Home Cortex jej může poznávat pouze nepřímo.

[FIXED] Rozdíl ve způsobu komunikace nesmí měnit základní význam zařízení ve `World Model`. Informace typu „pračka běží“, „program skončil“ nebo `Capability` typu „pozastavit program“ mají mít v doménovém modelu společný význam bez ohledu na to, zda byly získány či provedeny přes lokální API, cloud výrobce, Home Assistant, jinou integrační službu nebo budoucí rozhraní.

[FIXED] Přímá komunikace je další `Source` a případně cesta k `Actuator`; není automaticky absolutní pravdou ani automatickým oprávněním k `Action`. Data získaná přímo ze zařízení musí mít známý původ a mohou být porovnávána s nezávislými `Observation`. Ovládací příkaz musí stále respektovat `Capability`, `Constraint`, `Context`, `Purpose`, `Risk` a další pravidla rozhodování Home Cortex.

Příklad: pračka může přes výrobní API hlásit `RUNNING`, zatímco měření příkonu a vody poskytuje další nezávislou `Evidence`. Naopak ztráta cloudového spojení nesmí sama znamenat, že pračka přestala existovat nebo že její fyzický stav je neznámý ve všech ohledech; další `Source` mohou stále poskytovat informace.

## Integration Adapter

[PROVISIONAL] Pro oddělení doménového modelu od konkrétních výrobců a protokolů se předpokládá koncept `Integration Adapter`. Jeho úkolem je převádět konkrétní externí rozhraní na společné doménové pojmy Home Cortex a opačným směrem převádět povolené `Action` na konkrétní příkazy daného rozhraní.

Konceptuálně:

```text
výrobce / lokální API / cloud / HA / jiný protokol
                    │
            Integration Adapter
                    │
                    ▼
        společný model Home Cortex
 Device / Observation / Capability / Action / State
```

[FIXED] Specifické vlastnosti zařízení, které nelze beze ztráty významu převést na obecnou `Capability`, nesmí být kvůli sjednocení zahazovány. Společný model musí umožnit obecné schopnosti i rozšíření specifická pro určitý typ nebo konkrétní zařízení.

[OPEN] Není zatím rozhodnuto, zda jednotlivé `Integration Adapter` poběží přímo v procesu Home Cortex, jako samostatné moduly/služby, prostřednictvím Home Assistant, nebo kombinací těchto možností.

[TODO-DESIGN] Bude nutné definovat společný model `Device`, explicitní a odvozené identity, `Behavioral Signature`, `Power Signature`, stavů zařízení, `Capability`, dostupnosti komunikace, kvality `Source`, mapování výrobcem specifických funkcí a způsobu řešení výpadku nebo změny externího API.
