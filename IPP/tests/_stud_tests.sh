#!/usr/bin/env bash

# =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
# IPP - dka - doplňkové testy - 2016/2017
# =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
# Činnost: 
# - vytvoří výstupy studentovy úlohy v daném interpretu na základě sady testů
# =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
# Popis (README):
#
# Struktura skriptu _stud_tests.sh (v kódování UTF-8):
# Každý test zahrnuje až 4 soubory (vstupní soubor, případný druhý vstupní 
# soubor, výstupní soubor, soubor logující chybové výstupy *.err vypisované na 
# standardní chybový výstup (pro ilustraci) a soubor logující návratový kód 
# skriptu *.!!!). Pro spuštění testů je nutné do stejného adresáře zkopírovat i 
# váš skript. V komentářích u jednotlivých testů jsou uvedeny dodatečné 
# informace jako očekávaný návratový kód. 
# Tyto doplňující testy obsahují i několik testů rozšíření (viz konec skriptu).
#
# Proměnné ve skriptu _stud_tests.sh pro konfiguraci testů:
#  INTERPRETER - využívaný interpret 
#  EXTENSION - přípona souboru s vaším skriptem (jméno skriptu je dáno úlohou) 
#  LOCAL_IN_PATH/LOCAL_OUT_PATH - testování různých cest ke vstupním/výstupním
#    souborům
#  
# Další soubory archivu s doplňujícími testy:
# V adresáři ref-out najdete referenční soubory pro výstup (*.out nebo *.xml), 
# referenční soubory s návratovým kódem (*.!!!) a pro ukázku i soubory s 
# logovaným výstupem ze standardního chybového výstupu (*.err). Pokud některé 
# testy nevypisují nic na standardní výstup nebo na standardní chybový výstup, 
# tak může odpovídající soubor v adresáři chybět nebo mít nulovou velikost.
#
# Doporučení a poznámky k testování:
# Tento skript neobsahuje mechanismy pro automatické porovnávání výsledků vašeho 
# skriptu a výsledků referenčních (viz adresář ref-out). Pokud byste rádi 
# využili tohoto skriptu jako základ pro váš testovací rámec, tak doporučujeme 
# tento mechanismus doplnit.
# Dále doporučujeme testovat různé varianty relativních a absolutních cest 
# vstupních a výstupních souborů, k čemuž poslouží proměnné začínající 
# LOCAL_IN_PATH a LOCAL_OUT_PATH (neomezujte se pouze na zde uvedené triviální 
# varianty). 
# Výstupní soubory mohou při spouštění vašeho skriptu již existovat a pokud není 
# u zadání specifikováno jinak, tak se bez milosti přepíší!           
# Výstupní soubory nemusí existovat a pak je třeba je vytvořit!
# Pokud běh skriptu skončí s návratovou hodnotou různou od nuly, tak není 
# vytvoření souboru zadaného parametrem --output nutné, protože jeho obsah 
# stejně nelze považovat za validní.
# V testech se jako pokaždé určitě najdou nějaké chyby nebo nepřesnosti, takže 
# pokud nějakou chybu najdete, tak na ni prosím upozorněte ve fóru příslušné 
# úlohy (konstruktivní kritika bude pozitivně ohodnocena). Vyhrazujeme si také 
# právo testy měnit, opravovat a případně rozšiřovat, na což samozřejmě 
# upozorníme na fóru dané úlohy.
#
# =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

TASK=dka
INTERPRETER="php5.6"
EXTENSION=php
#INTERPRETER="python3.6"
#EXTENSION=py

# cesty ke vstupním a výstupním souborům
LOCAL_IN_PATH="./" # (simple relative path)
LOCAL_IN_PATH2="" #Alternative 1 (primitive relative path)
LOCAL_IN_PATH3="`pwd`/" #Alternative 2 (absolute path)
LOCAL_OUT_PATH="./" # (simple relative path)
LOCAL_OUT_PATH2="" #Alternative 1 (primitive relative path)
LOCAL_OUT_PATH3="`pwd`/" #Alternative 2 (absolute path)
# cesta pro ukládání chybového výstupu studentského skriptu
LOG_PATH="./"


# test01: mini definice automatu; Expected output: test01.out; Expected return code: 0
$INTERPRETER $TASK.$EXTENSION --input=${LOCAL_IN_PATH}test01.in --output=${LOCAL_OUT_PATH}test01.out --determinization 2> ${LOG_PATH}test01.err
echo -n $? > test01.!!!

# test02: jednoducha determinizace (z prednasek); Expected output: test02.out; Expected return code: 0
$INTERPRETER $TASK.$EXTENSION --input=${LOCAL_IN_PATH}test02.in -d > ${LOCAL_OUT_PATH}test02.out 2> ${LOG_PATH}test02.err
echo -n $? > test02.!!!

# test03: pokrocila determinizace; Expected output: test03.out; Expected return code: 0
$INTERPRETER $TASK.$EXTENSION -d --input=${LOCAL_IN_PATH}test03.in --output=${LOCAL_OUT_PATH}test03.out 2> ${LOG_PATH}test03.err
echo -n $? > test03.!!!

# test04: diakritika a UTF-8 na vstupu; Expected output: test04.out; Expected return code: 0
$INTERPRETER $TASK.$EXTENSION < "${LOCAL_IN_PATH3}test04.in" > ${LOCAL_OUT_PATH}test04.out 2> ${LOG_PATH}test04.err
echo -n $? > test04.!!!

# test05: slozitejsi formatovani vstupniho automatu (specialni znaky); Expected output: test05.out; Expected return code: 0
$INTERPRETER $TASK.$EXTENSION --input=${LOCAL_IN_PATH}test05.in --output=${LOCAL_OUT_PATH}test05.out 2> ${LOG_PATH}test05.err
echo -n $? > test05.!!!

# test06: vstup ze zadani; Expected output: test06.out; Expected return code: 0
$INTERPRETER $TASK.$EXTENSION -d --output="${LOCAL_OUT_PATH3}test06.out" < ${LOCAL_IN_PATH}test06.in 2> ${LOG_PATH}test06.err
echo -n $? > test06.!!!

# test07: odstraneni epsilon-prechodu (priklad z IFJ04); Expected output: test07.out; Expected return code: 0
$INTERPRETER $TASK.$EXTENSION -e --input=${LOCAL_IN_PATH}test07.in --output=${LOCAL_OUT_PATH2}test07.out 2> ${LOG_PATH}test07.err
echo -n $? > test07.!!!

# test08: Chybna kombinace parametru -e a -d; Expected output: test08.out; Expected return code: 1
$INTERPRETER $TASK.$EXTENSION --input=${LOCAL_IN_PATH}test08.in --output="${LOCAL_OUT_PATH3}test08.out" -d --no-epsilon-rules 2> ${LOG_PATH}test08.err
echo -n $? > test08.!!!

# test09: chyba vstupniho formatu; Expected output: test09.out; Expected return code: 40
$INTERPRETER $TASK.$EXTENSION --input=${LOCAL_IN_PATH2}test09.in --output=${LOCAL_OUT_PATH}test09.out 2> ${LOG_PATH}test09.err
echo -n $? > test09.!!!

# test10: semanticka chyba vstupniho formatu (poc. stav neni v mnozine stavu); Expected output: test10.out; Expected return code: 41
$INTERPRETER $TASK.$EXTENSION --input="${LOCAL_IN_PATH3}test10.in" --output=${LOCAL_OUT_PATH}test10.out 2> ${LOG_PATH}test10.err
echo -n $? > test10.!!!

# test90: Bonus RUL - prepinac --rules-only (bez rozsireni vraci chybu); Expected output: test90.out; Expected return code: 0
$INTERPRETER $TASK.$EXTENSION --input=${LOCAL_IN_PATH}test90.in --output=${LOCAL_OUT_PATH2}test90.out --rules-only 2> ${LOG_PATH}test90.err
echo -n $? > test90.!!!

# test91: Bonus WCH - oddeleni elementu nejen carkami, ale i bilymi znaky, symboly nemusi byt v apostrofech (bez rozsireni vraci chybu); Expected output: test91.out; Expected return code: 0
$INTERPRETER $TASK.$EXTENSION --input="${LOCAL_IN_PATH3}test91.in" --output=${LOCAL_OUT_PATH}test91.out --white-char 2> ${LOG_PATH}test91.err
echo -n $? > test91.!!!

# test92:  Bonus RUL a WCH - slozitejsi formatovani -r a -w (bez rozsireni vraci chybu); Expected output: test92.out; Expected return code: 0
$INTERPRETER $TASK.$EXTENSION -w --input=${LOCAL_IN_PATH}test92.in -r --output="${LOCAL_OUT_PATH3}test92.out" 2> ${LOG_PATH}test92.err
echo -n $? > test92.!!!
