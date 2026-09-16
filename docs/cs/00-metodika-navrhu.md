# Home Cortex – metodika návrhu

> Stav dokumentu: počáteční návrhová konvence

## Účel

Tento dokument stanovuje způsob, jakým bude Home Cortex navrhován a dokumentován před vznikem implementace.

Home Cortex má být nejprve přesně popsán jako systém: jeho pojmy, prvky, parametry, vztahy, vstupy, výstupy, chování, nejistoty, historie, důvody rozhodnutí a vazby mezi jednotlivými vrstvami. Implementace má následovat až poté, co budou její základní koncepty dostatečně definované.

## Autoritativní jazyk

[FIXED] Primárním a autoritativním jazykem specifikace je čeština.

Anglická dokumentace bude udržována jako významově odpovídající překlad české specifikace. Pokud by mezi českou a anglickou verzí vznikl rozpor, rozhodující je česká verze.

Překlad nesmí být mechanický na úkor významu. Musí zachovávat technický a doménový význam pojmů.

## Terminologie

[FIXED] Klíčové doménové a architektonické pojmy budou již v české specifikaci používány pod jednotným anglickým názvem, který je kandidátem pro pozdější použití v API, datovém modelu a implementaci.

Příklady: `World Model`, `World State`, `Event`, `Observation`, `Evidence`, `Confidence`, `Person`, `Presence`, `Activity`, `Intent`, `Context`, `Decision`, `Action`, `Task`, `Risk`, `Sensor`, `Actuator`, `Source`.

[FIXED] Pro projekt bude veden samostatný `Glossary`. Jeden koncept má mít jeden preferovaný název a přesnou definici. Synonyma nesmí nenápadně vytvářet nové významy.

## Stav návrhových tvrzení

Každé významné návrhové tvrzení může být označeno jedním z následujících stavů:

### [FIXED]

Základní architektonický princip nebo rozhodnutí, na kterém mohou být závislé další části návrhu. Lze jej rozšiřovat a zpřesňovat, ale změna jeho významu vyžaduje vědomé posouzení dopadů na celý návrh.

`FIXED` neznamená, že je změna navždy zakázána. Znamená, že nesmí být změněna lokálně a bez kontroly následků.

### [PROVISIONAL]

Aktuálně preferované řešení nebo předpoklad. Je dostatečně konkrétní pro další návrh, ale očekává se možnost jeho změny.

### [OPEN]

Otázka nebo problém, o kterém víme, ale zatím nemá rozhodnuté řešení.

### [TODO-DESIGN]

Místo, ke kterému se musíme během návrhu vědomě vrátit, typicky protože jeho definitivní řešení závisí na jiné dosud nenavržené části systému.

## Nehádat chybějící rozhodnutí

[FIXED] Dokumentace nesmí vytvářet zdání úplnosti tím, že nevyřešené zásadní otázky potichu doplní libovolným řešením.

Pokud řešení není známé, musí být označeno jako `OPEN`, `PROVISIONAL` nebo `TODO-DESIGN`. Nejasnost je v návrhové fázi přípustná; skryté rozhodnutí nikoliv.

## Decision Log / ADR

[FIXED] Významná architektonická rozhodnutí a jejich změny budou zaznamenávány v `Decision Log` formou ADR (Architecture Decision Record).

ADR má zachovat alespoň:

- původní stav nebo problém,
- přijaté rozhodnutí,
- důvod rozhodnutí,
- známé alternativy, pokud jsou relevantní,
- části specifikace, kterých se rozhodnutí týká,
- případné důsledky a nutné následné změny.

Cílem není pouze vědět, jak je Home Cortex navržen, ale také proč je tak navržen.

## Návrh před implementací

[FIXED] Před první skutečnou implementací musí být u základních prvků systému popsán alespoň jejich význam, odpovědnost, relevantní parametry, vstupy, výstupy, vztahy k ostatním prvkům a očekávané chování.

To neznamená požadavek na neměnný kompletní návrh celého systému před jakýmkoliv experimentem. Znamená to, že implementace nesmí nahrazovat chybějící doménové a architektonické přemýšlení.

## Otevřenost návrhu

[FIXED] Home Cortex je otevřený systém. Nové typy zdrojů informací, senzorů, akčních prvků, integrací, modelů a způsobů vyhodnocení musí být možné přidávat bez změny základního významu systému.

Otevřenost však neznamená absenci pravidel. Nový prvek musí být začleněn do společného doménového modelu a respektovat definované vztahy, původ dat, nejistotu, časovou platnost a další relevantní vlastnosti.
