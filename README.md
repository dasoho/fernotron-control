# fernotron-control
Control Fernotron shutters with FHEM and Raspberry Pi


Benötigte Hardware
------------------
433 MHz Sender/Empfänger Modul
- Getestet mit:
http://www.amazon.de/gp/product/B00OLI93IC/ref=as_li_tl?ie=UTF8&camp=1638&creative=19454&creativeASIN=B00OLI93IC&linkCode=as2&tag=epcblog-21&linkId=OAE7APMAS2T3VU6N

Hardwarevorbereitung Sendemodul
- Pin VCC des Sendemoduls an Pin 2 der GPIO Stiftleiste des Pi
- Pin GND des Sendemoduls an Pin 6 der GPIO Stiftleiste des Pi
- Pin ATAD des Sendemoduls an Pin 11 der GPIO Stiftleiste des Pi
- 17cm lange Antenne an den Antennenanschluß (ANT) des Sendemoduls löten

Hardwarevorbereitung Empfangsmodul
- Pin VCC des Empfangsmoduls an Pin 4 der GPIO Stiftleiste des Pi
- Pin GND des Empfangsmoduls an Pin 14 der GPIO Stiftleiste des Pi
- Pin DATA des Empfangsmoduls mit einem Spannungsteiler von 5V auf ca. 3,3V einstellen
  (z.B. 470 Ohm und 1 kOhm ergibt 3,4 V über dem 1 kOhm Widerstand)
  und mit Pin 13 der GPIO Stiftleiste des Pi verbinden
- 17cm lange Antenne an den Antennenanschluß (ANT) des Empfangsmoduls löten


Softwareinstallation
--------------------
Git installieren (falls noch nicht vorhanden)
- sudo apt-get update
- sudo apt-get install git-core

WiringPi installieren und kompillieren
- cd ~
- git clone git://git.drogon.net/wiringPi
- cd wiringPi
- ./build

Fernotron Software installieren und kompillieren
- cd ~
- git clone git://github.com/dasoho/fernotron-control.git
- cd fernotron-control
- make all

Damit ist die Softwareinstallation abgeschlossen.


Aufzeichnen der Fernbedienungscodes
-----------------------------------
Starten des Programms zur Aufzeichnung der Codes:
- cd ~/fernotron-control
- sudo ./FernotronSniffer

Nun die Fernotron Programmierzentrale **DIREKT** an die Antenne des Empfangsmoduls halten
und einen Rolladen auf-/bzw. abfahren.
Der Fernbedienungscode wird zyklisch auf der Konsole ausgegeben. Falls beim Empfang 
Fehler auftreten werden diese auch auf der Konsole angezeigt.

Senden der Fernbedienungscodes
------------------------------
Die aufgezeichneten Codes können mit Hilfe des Programmes FernotronSend gesendet werden:
- cd ~/fernotron-control
- sudo ./FernotronSend *code*

Je nach Code sollte nun der der entsprechende Rolladen auf- oder zufahren bzw. stoppen.

Einbindung in FHEM
------------------
Der Einfachheit halber sollten zunächst alle gewünschten Rolladenbewegungen in einem Skript
zusammengefaßt werden. Als Beispiel dient das Skript *FernotronRemote.sh*.
Die Einbindung in FHEM erfolgt über das Modul *GenShellSwitch.pm*, welches man sich serperat im
Internet besorgen muss. Das Modul wird in den Ordner fhem/FHEM kopiert und anschließend mit
reload *modulename* geladen.
Ein beipielhafte Konfiguration eines Rolladenschalters in FHEM sieht dann wie folgt aus:

- define Rolladen_WZ1 GenShellSwitch /home/pi/fernotron-control/FernotronRemote.sh 1 1 u d
- attr Rolladen_WZ1 cmdIcon on:black_up off:black_down

Dieser FHEM Schalter bewegt dann den ersten Rolladen der ersten Gruppe hoch bzw. runter.

Um die Rechteproblematik im Zusammenhang mit wiringPi zu umgehen kann man Eigentümer und Rechte
des Programms *FernotronSend* wie folgt abändern:
- sudo chown root ./FernotronSend
- sudo chmod 4711 ./FernotronSend

Einbindung in openHAB 1.x
-------------------------
Die Einbindung in openHAB ist sehr einfach: Es werden Items, Rules und natürlich die Sitemap benötigt:

Item-Definition (nur als virtueller Schalter definiert): 
```
Rollershutter   Rolladen_WZ_Spielplatz    "Spielplatz" <rollershutter>
```
Sitemap-Einbindung: 
```
Switch item=Rolladen_WZ_Spielplatz mappings=[UP="Rauf ", STOP="X", DOWN="Runter"]
```
Rule-Definition: 
```
rule Rolladen_WZ_Spielplatz
    when
        Item Rolladen_WZ_Spielplatz received command
    then
        switch (receivedCommand) {
            case UP:   (1..5).forEach[executeCommandLine("/usr/share/openhab/configurations/scripts/FernotronSend <code>",5000)]
            case STOP: (1..5).forEach[executeCommandLine("/usr/share/openhab/configurations/scripts/FernotronSend <code>",5000)]
            case DOWN: (1..5).forEach[executeCommandLine("/usr/share/openhab/configurations/scripts/FernotronSend <code>",5000)]
        }
end
```


