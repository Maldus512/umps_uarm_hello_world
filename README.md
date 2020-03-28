# Hello world configurabile per UARM/UMPS

Questa repository contiene un esempio di programma compilabile sia per l'emulatore uMPS2 (https://github.com/tjonjic/umps) che uARM (https://github.com/mellotanica/uARM). 
I due emulatori offrono librerie ROM e dispositivi mappati in memoria molto simili, per cui ottenere un risultato cross-platform e' relativamente semplice. Tramite delle macro `#ifdef` si includono gli header delle rispettive routine ROM e gli indirizzi dei dispositivi (in questo caso, solo il terminale).

A scopo di esempio sono implementati almeno due possibili metodi per la configurazione dell'architettura: make e scons.

## Requisiti

Perche' la compilazione vada a buon fine sono necessari i seguenti pacchetti:

- arm-none-eabi-gcc (vedere sezione sulle toolchain)
- mipsel-linux-gnu-gcc (vedere sezione sulle toolchain)
- uarm (per la compilazione su uarm)
- umps (per la compilazione su umps)
- make (per utilizzare i makefile)
- python-scons (per utilizzare SConstruct)
- python-kconfiglib (per utilizzare SConstruct)

## Make

Molto semplicemente vengono forniti due makefile separati per la compilazione, `uarmmake` e `umpsmake`. Invocando `make` sul file corrispondente si compila l'esempio per l'emulatore richiesto:

```
$ make -f uarmmake
$ make -f umpsmake
```

Dietro le quinte le differenze tra i due makefile sono:

 - utilizzo di un compilatore e di flag di compilazione appropriati
 - compilazione di diverse librerie di base
 - inclusione di diversi header
 - definizione delle macro `TARGET_UMPS` o `TARGET_UARM` per ottenere un comportamento diverso (in questo semplice esempio la cosa si riduce all'includere degli header diversi)

## CMake
L'attuale configurazione CMake, compatibile con entrambe le architetture, può essere presa come punto di partenza per la gestione di progetti più grandi. È possibile compilare per l'architettura uARM seguendo i passi seguenti, per uMPS sostituendo il file `toolchains/uarm.cmake` con `toolchains/umps.cmake`

```
$ mkdir build-uarm
$ cd build-uarm
$ cmake -D CMAKE_TOOLCHAIN_FILE=../toolchains/uarm.cmake ..
```

Quanto fatto costruirà un ambiente per CMake dentro la directory `build-uarm/` (il nome è arbitrario), dalla quale sarà poi possibile compilare i target desiderati, ad esempio

```
$ make kernel.uarm
```

Complessivamente, i file che appartengono a tale configurazione sono cinque: `CMakeLists.txt`, `uarm.cmake`, `umps.cmake`, `toolchains/uarm.cmake` e `toolchains/umps.cmake`.

L'utilità di CMake è dovuta alla possibilità di astrarre rispetto ai cosiddetti generatori (es. `make`) e alla sua ottima integrazione con molti degli IDE in circolazione.

### CMake: approfondimenti
A proposito del setup CMake, può essere utile osservare che

* I file all'interno di `toolchains/` non sono che "configurazioni" per le toolchain di cross-compilazione. Maggiori informazioni possono essere trovate presso la [documentazione ufficiale](https://cmake.org/cmake/help/v3.17/manual/cmake-toolchains.7.html).
* Può essere opportuno mirare all'uso di una versione CMake (vedi `cmake_minimum_required`) meno aggiornata rispetto all'ultima, nel tentativo di mantenere una più ampia compatibilità con le diverse installazioni.
* Benché questa configurazione abbia subito test su almeno tre distro Linux differenti, non è escluso che essa sappia adattarsi perfettamente alla vostra toolchain. In caso di errori è suggerito installarne una diversa con `crosstool-ng` (vedi sotto) o, eventualmente, aprire una discussione nella paggina Issues.

## Scons e Kconfig
Scons e' un build tool alternativo a make. Si tratta sostanzialmente di una libreria Python per la gestione di sorgenti. Invocando il comando `scons` viene eseguito lo script `SConstruct`, analogamente al funzionamento di make.
Usando i parametri `uarm` o `umps` e' possibile differenziare il target a riga di comando in maniera del tutto analoga al funzionamento di make.

```
$ scons umps
$ scons uarm
```

Oltre a questo semplice utilizzo pero' lo script `SConstruct` e' configurato anche per utilizzare il meccanismo di configurazione tipico del kernel Linux, Kconfig.
Kconfig comporta la definizione di menu di configurazione (i file `Kconfig`) che seguono una sintassi specifica (https://www.kernel.org/doc/html/latest/kbuild/kconfig-language.html). Molteplici tool sono poi in grado di leggere questi file e comportarsi di conseguenza o generare degli header che a loro volta influenzino il comportamento dei sorgenti.
Uno di questi strumenti e' `kconfiglib`, una libreria Python che fornisce sia delle interfacce (grafiche e non) per la modifica della configurazione che delle API per la gestione programmatica di quest'ultima. Essendo un tool in Python scons si puo' interfacciare direttamente a queste API, come viene fatto in questo esempio.

Per installare scons e `kconfiglib` si consiglia di appoggiarsi a un environment virtuale:

```
$ virtualenv .env
$ source .env/bin/activate
$ pip install -r requirements.txt
```

A questo punto e' possibile editare la configurazione (specificata dal file `Kconfig`) con il comando `guiconfig` o `menuconfig`. Una volta salvata una nuova configurazione lanciando il comando `scons` senza argomenti questa verra' usata per decidere il target di compilazione.

## Esecuzione

Per l'esecuzione dell'esempio fare riferimento ai manuali di uARM e uMPS2, rispettivamente.

## Toolchain

Sia `uarm-none-eabi-gcc` che `mipsel-linux-gnu-gcc` sono disponibili come pacchetti ufficiali nelle repository della maggior parte delle distribuzioni di Linux.
Nel caso non dovessere essere cosi', e' possibile creare dei binari ad-hoc tramite `crosstool-ng`.
`crosstool-ng` e' un tool per la compilazione di toolchain. Anch'esso dovrebbe essere disponibile come pacchetto (semi) ufficiale nella maggior parte delle distribuzioni; alternativamente e' installabile dai sorgenti (http://crosstool-ng.github.io/docs/install/).

Una volta installato e' possibile creare la toolchain necessaria, configurandola nel dettaglio. Per esempio, la configurazione di esempio `mipsel-unknown-linux-gnu` e' valida per compilare un kernel per umps2. Per generarla e usarla e' necessario:

- Installare `crosstool-ng`
- La directory dalla quale vengono lanciati i seguenti comandi non ha un significato particolare se non quello di contenere il file di configurazione e di compilazione intermedia del processo. Possono essere cancellati dopo aver creato la toolchain.
- Configurare la toolchain; per importare la configurazione di esempio basta invocare `ct-ng mipsel-unknown-linux-gnu`. Questo comando creera' il file `.config` nella cartella corrente.
- Costruire la toolchain con il comando `ct-ng build`. Questo comando scarichera' tutti i binari e i sorgenti necessari nella directory `.build`. Una volta terminato il processo (anche molto lungo a seconda delle prestazioni della macchina host), la toolchain sara' installata in `$HOME/x-tools/mipsel-unknown-linux-gnu/bin`
- Il cross-compiler cosi' generato deve essere usato da `make` o `scons` come compilatore per i sorgenti. Per esempio, lo script `umpsmake` dovrebbe essere modificato in questo modo:
```
#XT_PRG_PREFIX = mipsel-linux-gnu-
XT_PRG_PREFIX = ~/x-tools/mipsel-unknown-linux-gnu/bin/mipsel-unknown-linux-gnu-
```

- In seguito e' sufficiente invocare `make`
