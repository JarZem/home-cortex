# Home Cortex – Action, důsledky a hodnocení

> Tento dokument rozvíjí obecné principy rozhodování Home Cortex. Nezavádí konkrétní fyzikální ani matematický model. Definuje schopnosti, které musí být dostupné společně všem doménám domu.

## Action vždy mění svět

[FIXED] **Každá `Action` musí být posuzována podle svých očekávaných důsledků pro relevantní části `World State`, nikoli pouze podle bezprostředního zamýšleného účinku. Home Cortex musí umožnit reprezentovat účinky podporující jiné cíle, účinky konfliktní, vedlejší účinky, spotřebu zdrojů, vznik nebo změnu `Risk` a nejistotu těchto důsledků.**

Očekávané důsledky nemusí být všechny předem známy ani přesně vypočitatelné. Mohou pocházet z obecné znalosti, konfigurace konkrétního domu, pozorování, učení nebo odhadu a musí být možné vyjádřit jejich nejistotu. Neúplný model důsledků sám o sobě neznamená zákaz `Action`; musí však ovlivnit její hodnocení.

[FIXED] **Po provedení nebo naplánování `Action` musí její očekávané důsledky vstoupit do následné `Prediction`. Změna světa, která odpovídá očekávanému účinku vlastní `Action`, nesmí být bez dalšího interpretována jako nový nezávislý důvod pro protichůdnou `Action`.**

Tento princip má bránit tomu, aby několik lokálně správných rozhodnutí vytvářelo v celku oscilaci nebo vzájemné rušení. Typickým příkladem je současné větrání, vytápění, řízení vlhkosti a kvality vzduchu: jednotlivé zásahy mohou působit na stejné vlastnosti světa různými směry.

## Společné hodnocení napříč doménami

[FIXED] **Rozhodnutí různých domén nesmějí být prováděna izolovaně způsobem, který může vytvářet vzájemně se rušící nebo oscilující `Action`. Pokud více možných `Action` ovlivňuje společné části `World State`, musí být jejich očekávané důsledky možné vyhodnotit společně.**

Home Cortex proto nemá být složen z nezávislých řídicích systémů typu „topení rozhodne topit“, „větrání rozhodne větrat“ a „energie rozhodne topení omezit“, které následně bojují o stejný svět. Doménová znalost může poskytovat `Requirement`, `Constraint`, `Prediction`, `Risk`, možné účinky a další podklady, ale konečné rozhodnutí musí respektovat jejich společné působení.

[TODO-DESIGN] Bude nutné přesně definovat reprezentaci očekávaného účinku, podporujících a konfliktních účinků, vazeb mezi částmi `World State`, společného hodnocení více `Action` a mechanismus prevence nebo detekce rozhodovacích smyček.

## No Action je plnohodnotná alternativa

[FIXED] **`No Action` musí být při rozhodování plnohodnotnou alternativou. Zjištěná odchylka, možnost zlepšení nebo existence technické `Capability` sama o sobě nevytváří povinnost zasáhnout.**

Nečinnost neznamená nulový následek. Také při `No Action` se svět dále vyvíjí: prostor může chladnout, rostlina vysychat, koncentrace CO₂ růst nebo naopak může přechodná odchylka sama odeznít. Home Cortex proto porovnává očekávané budoucnosti při různých zásazích s očekávanou budoucností bez zásahu.

[FIXED] **Každá `Action` má důsledky a obecně pojatou cenu, která nemusí být finanční. Při hodnocení musí být možné zohlednit například energii, jiné omezené zdroje, opotřebení, čas, hluk, komfort, rušivost člověka, změnu rizika, ztrátu jiné užitečné vlastnosti nebo nejistotu výsledku. Stejným způsobem musí být hodnoceny i důsledky `No Action`.**

## Obecná podpora místo katalogu fyziky

[FIXED] **Základní architektura Home Cortex nesmí být závislá na existenci konkrétního fyzikálního, regulačního nebo matematického modelu. Musí poskytovat obecnou podporu pro popis a hodnocení stavu, cíle, míry dosažení cíle, očekávaných důsledků, nejistoty, omezení, zdrojů a obecně pojaté ceny alternativ. Konkrétní modely jsou vyměnitelnými zdroji těchto informací, nikoli základem architektury.**

To umožňuje použít stejný rozhodovací rámec pro vytápění, větrání, osvětlení, stínění, zavlažování, energii, kvalitu vzduchu nebo jiné oblasti bez nutnosti předem definovat stovky zvláštních fyzikálních pravidel v jádru systému.

## Příklad: žaluzie jako víceúčelový Actuator

Žaluzie ukazují, proč nelze význam `Action` odvodit pouze z typu zařízení. Stejná změna polohy může v různém `Context` plnit odlišný `Purpose` a současně vytvářet několik dalších důsledků.

Večer může být zatažení žádoucí kvůli soukromí. Přes den může otevření umožnit využití přirozeného světla pro člověka nebo rostliny. V zimě může dopadající Slunce představovat užitečný tepelný zisk, zatímco zatažená žaluzie může v jiné části dne snížit tepelnou ztrátu jako další izolační vrstva. V létě může naopak omezení slunečního záření snižovat přehřívání. Potřeba světla navíc nemusí být určena okamžitou hodnotou; u některých účelů může být relevantní kumulativní expozice za určité období.

Proto nelze obecné rozhodování redukovat na pravidla typu „ráno vytáhni“ nebo „večer zatáhni“. Relevantní mohou být `Purpose`, `Preference`, soukromí, přítomnost osob, potřeby rostlin, venkovní a vnitřní podmínky, poloha Slunce, očekávaný tepelný vývoj, roční období, historie expozice, spotřeba energie, očekávaný budoucí `Context` a další důsledky. Konkrétní fyzikální výpočet těchto vztahů není součástí tohoto fundamentu.

[TODO-DESIGN] Při návrhu obecného modelu bude nutné vyřešit také požadavky závislé na historii nebo kumulaci v čase, například denní světelnou expozici rostlin, dlouhodobou expozici vlhkosti, kvalitu vzduchu, spotřebu energie nebo jiné veličiny, jejichž význam nelze určit pouze z okamžitého `World State`.