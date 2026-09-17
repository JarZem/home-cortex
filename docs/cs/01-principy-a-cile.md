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

## Target State a řízení budoucího stavu

[FIXED] **Home Cortex má primárně určovat a vyhodnocovat žádoucí budoucí stavy relevantní části světa, nikoli předem definované posloupnosti akcí. `Action` je prostředkem přechodu mezi současným nebo očekávaným `World State` a přípustným `Target State`.**

[FIXED] **`Target State` nemusí být jediná hodnota. Může představovat množinu přípustných stavů určenou současnými `Purpose`, `Preference`, `Constraint`, `Risk`, fyzikálními vztahy a očekávaným budoucím `Context`. Pokud existuje více přípustných stavů, Home Cortex může mezi nimi optimalizovat například spotřebu energie, náklady nebo jiné zdroje.**

[FIXED] **Pokud má změna světa významnou setrvačnost nebo zpoždění, Home Cortex nesmí čekat na vznik `Discrepancy`. Musí být schopen porovnat `Prediction` budoucího `World State` s budoucím `Target State` a zahájit vhodnou `Action` s potřebným předstihem.**

[FIXED] **Existence `Discrepancy` mezi aktuálním nebo předpovězeným `World State` a `Target State` sama o sobě neopravňuje Home Cortex k okamžité kompenzační `Action`. Před reakcí musí být zohledněn `Context`, aktivní `Purpose`, příčina nebo pravděpodobná příčina odchylky, její očekávané trvání a důsledky zásahu i nečinnosti.**

`Target State` tedy nemusí popisovat příkaz zařízení. Například požadavkem může být dosažení vhodných tepelných podmínek pro konkrétní osoby v očekávaném čase; zapnutí zdroje tepla, změna výkonu nebo jiná konkrétní `Action` je až prostředkem, jak tohoto stavu dosáhnout. Obdobně může být cílem přijatelná vlhkost nebo riziko kondenzace, nikoli pevně stanovená doba běhu ventilátoru.

Při hledání přípustného `Target State` mohou současně působit různé a někdy konfliktní požadavky. Například tepelný stav prostoru může být ovlivněn přítomností více osob, jejich rozdílnými `Preference`, spánkem, sprchováním, přítomností zvířat, požadavky rostlin, ochranou budovy, očekávanou dobou nepřítomnosti a ekonomickými cíli. Výsledkem proto nemusí být jedna univerzální „komfortní“ nebo „úsporná“ hodnota.

[TODO-DESIGN] Bude nutné přesně definovat reprezentaci `Target State`, jeho časovou platnost, toleranci a podmínky dokončení, skládání více současných požadavků, řešení konfliktů a optimalizační kritéria. Samostatně bude nutné navrhnout plánování v čase pro systémy s významnou setrvačností a možnost průběžného přepočtu plánu při změně `World State`, `Prediction` nebo `Context`.

## Rozhodování mezi možnými budoucnostmi

[FIXED] **Home Cortex nesmí být založen na sbírce pevných hranic pro všechny možné situace. Musí poskytovat obecný mechanismus pro porovnávání přípustných alternativních stavů, trajektorií a jejich očekávaných následků podle aktuálního `Context`, `Purpose`, `Preference`, `Constraint`, `Risk`, dostupných zdrojů a nejistoty.**

[FIXED] **Při rozhodování za nejistoty nesmí Home Cortex hodnotit pouze pravděpodobnost, že se `Prediction` mýlí. Musí zohlednit také rozdílné důsledky jednotlivých možných omylů. Dvě stejně pravděpodobné chyby proto nemusí mít stejnou závažnost ani vést ke stejnému `Decision`.**

[FIXED] **Home Cortex má při volbě současné `Action` zohlednit více realistických budoucích vývojů, pokud jsou pro rozhodnutí významné. Nemá slepě optimalizovat pouze nejpravděpodobnější budoucnost; má preferovat řešení, jehož očekávané důsledky jsou přijatelné napříč relevantními možnostmi a které současně co nejlépe plní platné cíle a omezení.**

To umožňuje pracovat s asymetrií následků. Například malá pravděpodobnost mírně zbytečné spotřeby energie může být přijatelnější než podobně pravděpodobný, ale významný zásah do komfortu člověka; v jiné situaci může naopak dlouhodobé plýtvání převážit nad zanedbatelným rozdílem komfortu. Bezpečnostní `Constraint` přitom může některé varianty z množiny přípustných řešení úplně vyloučit, místo aby byl bezpečnostní dopad pouze další položkou v optimalizačním skóre.

Hodnotící kritérium nesmí být omezeno na finanční cenu. Podle řešené oblasti mohou být relevantní například komfort, energie, náklady, čas, opotřebení zařízení, hluk, kvalita prostředí, rušivost pro člověka, spotřeba jiných zdrojů, nejistota výsledku nebo očekávaný `Risk`.

Příklad: pokles teploty během úmyslného větrání není sám o sobě důvodem zvýšit výkon topení. Stejná teplotní odchylka při zavřených oknech a očekávaném příchodu člověka může mít zcela jiný význam. Rozhodující není pouze velikost odchylky, ale její příčina, účel probíhající změny, očekávané trvání a následky dostupných reakcí.

[TODO-DESIGN] Bude nutné definovat obecnou reprezentaci hodnotících kritérií a jejich skládání, aniž by byl systém vázán na jednu univerzální číselnou „cenu“. Zvlášť bude nutné oddělit nepřekročitelné `Constraint`, měkké `Preference`, optimalizační cíle, toleranci, nejistotu a hodnocení následků různých trajektorií.

## Očekávaný a pozorovaný svět

[FIXED] **Home Cortex musí rozlišovat mezi tím, jaký stav světa očekává na základě modelu, a tím, jaký stav světa vyplývá z aktuálních `Observation`. Rozdíl mezi očekáváním a pozorováním je sám o sobě `Evidence`.**

`World Model` může vytvářet `Expectation` o současném stavu a `Prediction` o možném budoucím stavu. Tyto výsledky nesmí být zaměněny za přímo pozorovanou skutečnost.

Pokud se očekávaný a pozorovaný stav významně liší, vzniká `Discrepancy`. `Discrepancy` není automaticky důkazem poruchy. Může znamenat chybný nebo neúplný model, neočekávanou vnější podmínku, vadné či nevhodně interpretované měření nebo dosud neznámou příčinu. Samotný rozpor je však novou `Evidence`, kterou musí být možné dále vyhodnotit.

Příklad: astronomický model může spolehlivě určit, že je Slunce nad horizontem, ale z toho nevyplývá skutečná úroveň osvětlení. Bouřka, oblačnost, mlha, okolní překážky nebo jiné podmínky mohou způsobit, že pozorované světelné podmínky výrazně neodpovídají jednoduchému očekávání založenému pouze na poloze Slunce.

[TODO-DESIGN] Bude nutné přesně definovat vztahy mezi `Expectation`, `Prediction`, `Observation`, `Evidence`, `Discrepancy`, `Confidence` a časovou platností těchto tvrzení.

## Fyzický stav, vnímání a preference člověka

[FIXED] **Home Cortex musí oddělovat objektivně popisované vlastnosti světa od jejich subjektivního vnímání a preferencí jednotlivých osob. Stejný fyzický `World State` může pro různé osoby vytvářet rozdílné `Perception` a vést k rozdílným potřebám.**

Fyzikální veličina sama o sobě neurčuje lidský komfort ani požadovanou reakci systému. Například stejná teplota může být jednou osobou vnímána jako příjemná a jinou jako chladná. Vnímání stejné osoby se navíc může měnit podle jejího aktuálního stavu a `Context`, například po koupeli, při fyzické aktivitě nebo po dlouhém nehybném sezení. Obdobně skutečná intenzita osvětlení není sama o sobě odpovědí na otázku, zda je světla dostatek; záleží mimo jiné na osobě, činnosti a místě.

[FIXED] `Perception` a `Preference` musí být vztahovány ke konkrétní osobě a relevantnímu `Context`. Nesmí být ukládány jako obecná vlastnost prostoru jen proto, že byly v určité situaci platné pro jednoho člověka.

[FIXED] Home Cortex má využívat historii k postupnému vytváření časově a kontextově závislého modelu toho, kde se jednotlivé osoby obvykle nacházejí, jaké činnosti v daných místech a časech vykonávají a jaké podmínky jim v těchto situacích vyhovují. Tento model není pevným rozvrhem; jde o naučené vztahy s odpovídající mírou `Confidence`, které se mohou v čase měnit.

Takový model může například rozlišovat, že stejná osoba preferuje jiné tepelné nebo světelné podmínky při sledování televize, po koupeli, při práci, při spánku nebo při pohybu domem. Stejně tak může zachytit rozdíly mezi jednotlivými osobami.

[TODO-DESIGN] Přesný model `Perception`, `Preference`, osobního komfortu a časově-kontextových profilů bude definován samostatně. Musí být navázán na `Person`, `Activity`, `Context`, historii a `Confidence` a musí umět zachytit změnu preferencí v čase.

## Historie, korelace a kauzalita

[FIXED] Pro Home Cortex není důležitý pouze současný stav, ale také způsob, jakým tento stav vznikl.

Například informace `window.bedroom = CLOSED` sama nemusí dostatečně popisovat situaci. Relevantní může být posloupnost událostí, která jí předcházela: zjištěné riziko přicházející bouřky, upozornění člověka, jeho přesun mezi místnostmi a následné fyzické zavření okna.

Historie proto není pouze diagnostický log. Je zdrojem `Evidence`, vysvětlení současného `World State` a budoucích trénovacích dat.

[FIXED] **Opakovaná časová nebo statistická korelace sama o sobě nesmí vytvořit automatickou `Action`. Home Cortex se musí snažit rozpoznat podmínky, události a kauzální nebo kontextové vztahy, které pozorovanému jednání předcházejí.**

Pravidelnost času, místa nebo opakování může být `Evidence` pro určitou hypotézu nebo `Context`, ale nesmí být bez dalšího zaměněna za příčinu. Pokud například osoba opakovaně přichází kolem půlnoci do ložnice a rozsvítí malou lampu, systém se nemá naučit „o půlnoci rozsvítit lampu“. Má hledat relevantní předcházející stav, například skutečný příchod osoby do ložnice, nedostatek světla, probíhající přechod ke spánku, přítomnost další spící osoby nebo jiné podmínky.

[FIXED] Naučený vzorec může být podkladem pro `Prediction`, `Expectation`, zvýšení `Confidence`, aktivní získání další informace nebo vytvoření hypotézy. Samotná pravidelnost však nestačí k oprávnění fyzické automatické `Action`, pokud nejsou splněny její kontextové a bezpečnostní podmínky.

[TODO-DESIGN] Přesný model historie, neměnných `Event`, odvozených stavů, kauzálních a korelačních vztahů a možnosti zpětného přehrání (`Replay`) bude definován samostatně.

## Rozhodování není mapa automatizací

[FIXED] **Home Cortex nesmí být založen na předem vytvořené mapě „událost → sada podmínek → akce“. Rozhodování má vycházet z `World State`, vztahů mezi entitami, `Intent`, `Purpose`, očekávaného vývoje, podmínek dokončení, `Constraint`, `Risk` a dostupných `Evidence`. Konkrétní lidská `Activity` je zdrojem změny a očekávání v `World Model`, nikoli sama o sobě názvem automatizace.**

[FIXED] **Neznámý pojem nebo `Activity` nesmí Home Cortex nutit k domýšlení chybějícího významu. Má pracovat s tím, co z informace skutečně dokáže odvodit, zachovat nejistotu a aktivně vyžádat pouze takovou chybějící informaci, která je významná pro aktuální `Decision`.**

Výrok člověka proto nemusí být předem známým názvem scénáře. Například věta „jdu venčit psa“ může být užitečná i tehdy, pokud Home Cortex nepotřebuje rozumět pojmu pes ani znát konkrétní význam venčení. Následná `Observation` mohou ukázat posloupnost přesunu osoby z obýváku přes verandu ven z domu a pozdější návrat. Opakováním podobných posloupností může vznikat model očekávaného přesunu, pravděpodobného návratu a jeho časového rozložení.

Takto naučené očekávání může být dále podmíněno `Context`. Doba návratu může například statisticky souviset s počasím, teplotou, deštěm, mrazem nebo jinými pozorovanými podmínkami. Tyto vztahy jsou `Evidence` pro `Prediction`; nesmějí být automaticky prohlášeny za příčinu jen na základě korelace.

[FIXED] Home Cortex má při rozhodování pracovat s významem informace, který je pro dané rozhodnutí relevantní, nikoli vyžadovat úplné sémantické pochopení každého pojmu. Pokud pro bezpečné nebo významné rozhodnutí některá informace skutečně chybí, může ji aktivně získat od člověka nebo jiného `Source`.

Příklad: pokud člověk oznámí záměr odejít z domu a současně probíhá pečení, nemusí být správnou reakcí troubu automaticky vypnout. Home Cortex může podle dostupného `Context`, `Purpose`, časového omezení a bezpečnostních `Constraint` ponechat stav beze změny, nebo se uživatele stručně zeptat, zda má trouba po odchodu pokračovat a případně do kdy. Stejný princip může být v budoucnu realizován hlasovým rozhraním, mobilním rozhraním, Home Assistantem nebo lokálním dotykovým panelem.

[TODO-DESIGN] Bude nutné přesně definovat reprezentaci podmínek dokončení, časově platných záměrů, očekávaných přechodů mezi stavy a mechanismus, kterým Home Cortex rozhodne, zda je hodnota chybějící informace dostatečně významná pro aktivní dotaz na člověka.

## Actuator, Capability a omezení Action

[FIXED] **Skutečnost, že `Actuator` technicky umožňuje určitou `Action`, neznamená, že Home Cortex smí tuto `Action` v libovolném `Context` provést. Technická `Capability` musí být oddělena od podmínek přípustnosti a bezpečnosti akce.**

Každý typ ovládaného zařízení musí mít popsané své obecné `Capability` a obecná pravidla či `Constraint`, která určují, za jakých okolností je konkrétní `Action` přípustná, zakázaná, povinná nebo vyžaduje další ověření. Nemá být nutné ručně vytvářet kompletní rozhodovací logiku pro každý jednotlivý kus zařízení; jednotlivé instance mají pokud možno dědit obecnou sémantiku svého typu a doplňovat pouze své specifické vlastnosti, umístění, účel a výjimky.

Například světlo, kávovar, lednice, chytrá zásuvka nebo jiný spotřebič mohou mít odlišné obecné vlastnosti a omezení. Konkrétní zařízení pak může přidávat další význam – například světlo určené pro rostliny může mít jiný `Purpose` než světlo určené lidem.

[FIXED] **Automatická `Action` nesmí bez dostatečného důvodu zhoršit podmínky jiné přítomné osoby nebo zmařit její zjevný či důvodně předpokládaný `Intent`.** Home Cortex proto musí před zásahem vyhodnocovat osoby a činnosti, kterých se změna dotkne, nikoli pouze osobu nebo událost, která rozhodování vyvolala.

Příklad: odchod jedné osoby z kuchyně není dostatečný důvod pro zhasnutí, pokud v kuchyni zůstává jiná osoba. Naopak zhasnutí v prokazatelně prázdné místnosti může být obecně přípustné, pokud světlo nemá jiný aktivní `Purpose`, například osvětlení rostlin.

[FIXED] `Constraint` může mít různou sílu. Některé podmínky pouze ovlivňují vhodnost nebo preferenci akce, jiné ji zakazují a bezpečnostní podmínky mohou určitou `Action` naopak učinit povinnou. Bezpečnostní omezení musí mít možnost mít vyšší prioritu než komfort, běžná preference nebo naučený zvyk.

Příklad: pokud je žehlička napájena přes ovladatelnou chytrou zásuvku a `World Model` spolehlivě určí, že v domě nikdo není, může být pro tuto konkrétní kombinaci zařízení a zapojení definován bezpečnostní `Constraint`, podle kterého napájení žehličky nesmí zůstat zapnuté. V takovém případě nejde o naučenou preferenci, ale o povinnou bezpečnostní reakci. Přesná definice musí současně respektovat jistotu informace o nepřítomnosti a skutečnou schopnost daného `Actuator` bezpečně napájení odpojit.

[FIXED] Každá významná automatická `Action` musí být před provedením vyhodnotitelná alespoň vůči své `Capability`, relevantním `Constraint`, aktuálnímu `Context`, dotčeným osobám, aktivnímu `Purpose`, známým `Risk` a požadované míře jistoty vstupních informací.

[TODO-DESIGN] Bude vytvořen obecný model typů zařízení, `Capability`, `Constraint`, `Purpose`, priorit, konfliktů mezi pravidly a dědičnosti obecných pravidel do konkrétních instancí `Actuator`. Zvlášť bude nutné definovat, jak se řeší konflikt komfortu, preference, uživatelského příkazu, provozního účelu a bezpečnosti.

## Zdroje informací nejsou pouze fyzické senzory

[FIXED] Home Cortex nesmí chápat svět pouze prostřednictvím fyzických `Sensor` instalovaných v domě.

`Source` může poskytovat `Observation` o světě bez ohledu na to, zda jde o fyzické čidlo, jiný informační systém nebo externí službu.

Příklady zahrnují fyzické senzory, Home Assistant, Zigbee zařízení, síťovou infrastrukturu, telefon, čas, kalendářní informace, předpověď počasí, radar srážek nebo službu detekce blesků.

[FIXED] Home Cortex musí být schopen kombinovat více různých `Source` a posuzovat jejich informace v kontextu, včetně jejich nejistoty, stáří a vzájemné podpory nebo rozporu.

## Reakce nemusí být pouze automatická

[FIXED] Výsledkem `Decision` nemusí být pouze automatická `Action` provedená akčním prvkem.

Pokud systém nemá vhodný `Actuator`, může být vykonavatelem člověk. Home Cortex musí být schopen vytvořit požadavek, upozornění, `Task` nebo `Risk`, předat jej vhodnému člověku a následně sledovat, zda byl problém skutečně vyřešen.

Potvrzení člověka a skutečné vyřešení problému jsou dvě rozdílné skutečnosti.

## Aktivní získávání informací a interakce s člověkem

[FIXED] Home Cortex není pouze pasivním příjemcem informací. Pokud je pro rozhodnutí potřeba další informace a existuje způsob, jak ji získat, může systém aktivně změnit způsob pozorování světa.

To může například znamenat dočasné zapnutí energeticky náročnějšího `Sensor`, změnu frekvence měření nebo vyžádání další informace z dostupného `Source`.

[FIXED] **Home Cortex nesmí vyžadovat interakci s člověkem pouze proto, že má nízkou `Confidence` nebo neúplný `World Model`. Aktivní dotaz má vzniknout tehdy, když získaná informace může významně změnit důležité `Decision`, zejména při relevantním `Risk`, možném významném zásahu do záměru člověka nebo při cíleném učení, ke kterému člověk dal prostor.**

[FIXED] **Při získávání chybějící informace má Home Cortex postupovat v tomto pořadí: nejdříve pozoruj → potom odvozuj → pokud můžeš bezpečně rozhodnout, rozhodni → pokud chybějící informace není důležitá, toleruj nejistotu → pokud důležitá je, zvol nejméně obtěžující vhodnou interakci → teprve potom se zeptej.**

Nejistota tedy sama o sobě není důvodem k vyrušování člověka. Home Cortex má umět ponechat část `World Model` neúplnou nebo nejistou, pokud tato neznalost nemá významný dopad na aktuální rozhodnutí, bezpečnost nebo jiný důležitý cíl.

[TODO-DESIGN] Způsob rozhodování o hodnotě informace, energetické ceně měření, aktivním řízení senzorů a ceně či rušivosti interakce s člověkem bude definován v samostatné části. Bude také nutné definovat výběr vhodného komunikačního kanálu podle `Context`, naléhavosti, dostupnosti člověka, spolehlivosti kanálu a požadované rychlosti odpovědi.

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