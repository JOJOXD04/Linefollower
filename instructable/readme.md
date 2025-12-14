# Instructable

Een instructable is een stappenplan - zonder verdere uitleg - hoe je vertrekkend van de bill of materials en gebruik makend van de technische tekeningen de robot kan nabouwen. Ook de nodige stappen om de microcontroller te compileren en te uploaden staan beschreven.  

### stap 1
Bestel alle componenten uit de bill of materials. Zoveel mogelijk van dezelfde leverancier (OTRONIC).
### stap 2
Zet de componenten op het breadbord zoals op de foto. Dit is het gemakkelijkst om te testen en om daarna op ons frame te monteren.

<img width="300" height="380" alt="image" src="https://github.com/user-attachments/assets/8aed4a63-c7ba-4c46-9196-969f40b37a0b" />

### stap 3
Test de componenten uit a.d.h.v de schemas en codes onder de tab code -> proof of concepts. Deze zullen een goede werking demonstreren van de componenten.

Sensoren worden best op een vaste hoogte gehouden. Om een duidelijk verschil te zien.
### stap 4
Voor het programma te uploaden naar de arduino. Moet de juiste arduino gelinkt worden aan de com poort waar de nano v3 arduino mee verbonden is in arduino IDE. Hiervoor moet eerst de bibliotheek **Arduino AVR boards** geinstalleerd worden. Ga hiervoor naar Tools helemaal vanboven op het scherm -> boards -> boards manager. Als het geinstalleerd is kan de juiste com poort veranderd worden naar de naam van de microcontroller. Om de juiste COM poort te vinden ga naar apparaatbeheer (op de laptop/pc) -> poort -> Hier vindt u de naam USB-SERIAL CH340 dit is de microcontroller. Ga nu naar arduino IDE en verbind de juiste COM poort met de naam **Arduino nano**. Als dit gedaan is kan de code geupload worden naar de Nano V3 arduino.
### stap 5
Print het frame dat te vinden is onder technische tekeningen -> mechanisch. Hier kan het stl bestand gevonden worden. 
### stap 6
Zaag een stuk van een slotbout zet er 2 rondellen van 1mm dikte op (wordt gebruikt als volgwiel). Steek het in het gat dat in het frame zit met de bolle kant naar beneden.

<img width="585" height="175" alt="image" src="https://github.com/user-attachments/assets/9c9fe35b-bca3-4c00-85b6-f7c0916b7a52" />

### stap 7
Maak nu een gleuf 5mm van de batterij houder box voor de sensor. Groot genoeg zodat de draden erin kunnen van de sensor. De sensor zat eerst helemaal vanvoor maar dit geeft problemen voor te korte bochten. Daarom word de sensor zo dicht mogelijk geplaatst bij de wielen.
### stap 8
Neem een lijmpistool en lijm de motoren en de sensor array aan het frame.
### stap 9
Verbind nu alles met elkaar, Zoals op het schema. Dit staat onder technische tekeningen -> elektronisch.
### stap 10 







