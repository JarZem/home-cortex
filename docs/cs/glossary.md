# Home Cortex – Glossary

> Autoritativní slovník doménových a architektonických pojmů. Definice jsou zatím počáteční a budou zpřesňovány spolu s návrhem.

## Zásady slovníku

[FIXED] Jeden doménový koncept má mít jeden preferovaný anglický název použitelný později také v implementaci.

[FIXED] Změna významu již používaného pojmu musí být posouzena vůči všem částem specifikace, které jej používají.

## Pojmy

### World Model

[PROVISIONAL] Strukturovaný model relevantní části reálného světa, kterou Home Cortex potřebuje znát pro své vyhodnocování a rozhodování. Zahrnuje entity, jejich vlastnosti, vztahy a pravidla potřebná pro interpretaci pozorování.

### World State

[PROVISIONAL] Aktuální vyhodnocená podoba `World Model` v určitém čase. Může obsahovat přímo známé i odvozené skutečnosti a jejich `Confidence`.

### Event

[TODO-DESIGN] Časově ukotvený záznam skutečnosti významné pro Home Cortex. Je nutné přesně oddělit surovou událost, pozorování, odvozenou událost a změnu stavu.

### Fluent

[PROVISIONAL] Stav nebo vlastnost, která může platit po určitou dobu a jejíž platnost může být zahájena, změněna nebo ukončena událostmi či jinými skutečnostmi. `Fluent` umožňuje odlišit okamžik typu „něco nastalo“ od tvrzení typu „něco po určitou dobu platí“.

### Observation

[PROVISIONAL] Informace o světě získaná z konkrétního `Source`. `Observation` není automaticky pravda; může mít kvalitu, nejistotu, stáří a další metadata.

### Evidence

[PROVISIONAL] Informace použitelná pro podporu nebo oslabení určité hypotézy, odvozeného stavu nebo rozhodnutí. Jedna `Observation` může být `Evidence` pro více různých tvrzení.

### Expectation

[PROVISIONAL] Očekávaný současný nebo následný stav odvozený z `World Model`, známých vztahů a dostupného `Context`. `Expectation` není `Observation` a nesmí být vydávána za přímo zjištěnou skutečnost.

### Prediction

[PROVISIONAL] Odhad budoucího stavu nebo události a případně jejího času či časového intervalu, vytvořený na základě `World Model`, historie, současného `World State` a dalších dostupných informací. Může mít vlastní `Confidence`.

### Discrepancy

[PROVISIONAL] Významný rozpor mezi očekávaným stavem (`Expectation` nebo relevantní `Prediction`) a stavem vyplývajícím z `Observation`. `Discrepancy` je sama o sobě `Evidence`; její příčinou může být například neúplný model, neočekávaná podmínka, chyba měření nebo dosud neznámá skutečnost.

### Confidence

[PROVISIONAL] Vyjádření míry jistoty, kterou Home Cortex přiřazuje odvozenému tvrzení nebo stavu. Přesná matematická interpretace a rozsah budou definovány později.

### Source

[PROVISIONAL] Původ informace poskytující Home Cortex `Observation`. Může jít o fyzický senzor, zařízení, jiný informační systém nebo externí službu.

### Sensor

[PROVISIONAL] Fyzický nebo logický prostředek měření či detekce, jehož výstup může prostřednictvím `Source` vytvářet `Observation`. Přesný vztah `Sensor` a `Source` bude ještě zpřesněn.

### Actuator

[PROVISIONAL] Prvek schopný na základě příkazu fyzicky nebo logicky změnit stav části světa nebo systému.

### Person

[TODO-DESIGN] Reprezentace člověka relevantního pro `World Model`, jeho identitu, přítomnost, polohu, stav a vztah k domu. Rozsah osobních údajů bude definován později.

### Presence

[PROVISIONAL] Odhad přítomnosti osoby nebo obecně osoby v určité oblasti či v domě, typicky vyjádřený s `Confidence`.

### Activity

[PROVISIONAL] Odvozená činnost probíhající v určitém kontextu, například práce, spánek, sledování televize nebo přesun mezi prostory.

### Perception

[PROVISIONAL] Odhad toho, jak konkrétní `Person` v daném `Context` subjektivně vnímá určitý fyzický stav nebo podmínku. `Perception` není fyzikální veličina ani obecná vlastnost prostoru a může se lišit mezi osobami i u stejné osoby podle činnosti, předchozího stavu, času a dalších okolností.

### Preference

[PROVISIONAL] Individuální a kontextově závislá informace o tom, jaký stav nebo rozsah podmínek konkrétní `Person` v dané situaci preferuje. `Preference` se může v čase měnit a může být explicitně sdělena, odvozena z chování nebo postupně naučena; její původ a `Confidence` musí být rozlišitelné.

### Intent

[PROVISIONAL] Odhad toho, čeho chce osoba nebo systém v daném `Context` dosáhnout. `Intent` nesmí být zaměňován s přímo pozorovanou skutečností.

### Context

[TODO-DESIGN] Soubor relevantních skutečností, historie a podmínek použitých pro interpretaci události nebo rozhodování. Přesná hranice vůči `World State` bude definována později.

### Decision

[PROVISIONAL] Výsledek vyhodnocení, že vzhledem k aktuálním informacím má nebo nemá následovat určitá reakce. `Decision` musí být možné vztáhnout k důvodům a použitým `Evidence`.

### Action

[PROVISIONAL] Konkrétní reakce iniciovaná Home Cortex, která má změnit stav, získat další informaci nebo komunikovat s člověkem či jiným systémem.

### Task

[PROVISIONAL] Požadavek na provedení činnosti, který může mít vykonavatele, prioritu, důvod, termín a stav splnění. Vykonavatelem může být člověk nebo systém podle možností daného úkolu.

### Risk

[PROVISIONAL] Rozpoznaný nebo předpokládaný nežádoucí budoucí stav či možnost škody, která může vyžadovat `Decision`, `Action` nebo vytvoření `Task`.

### Replay

[PROVISIONAL] Opětovné zpracování historických vstupů novou nebo změněnou verzí vyhodnocovací logiky za účelem rekonstrukce, testování nebo porovnání výsledků.

## Otevřené pojmy

[TODO-DESIGN] Slovník bude průběžně rozšiřován. Zejména bude potřeba přesně definovat pojmy pro prostorový model domu, zařízení, schopnosti zařízení, příkaz, původ příkazu, korelaci událostí, časovou platnost, kvalitu dat, hypotézu, odvozený stav, aktivní měření, člověkem potvrzenou pravdu, osobní časově-kontextový profil a trénovací vzorek.
