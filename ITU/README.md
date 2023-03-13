# Zvířecí útulek Happy paw
#### Týmový projekt VUT FIT 


## Autoři
- Samuel Kuchta xkucht11@stud.fit.vutbr.cz
- Jakub Kováčik xkovac58@stud.fit.vutbr.cz
- Adam Dalibor Jurčík xjurci08@stud.fit.vutbr.cz

## Dokumentace
nachází se v technické zprávě [`./doc/Technicka_zprava_final.docx`](./doc/Technicka_zprava_final.docx)

## Struktura projektu
- celá webová aplikace se nachází v `./website`
- spuštění se provádí díky `./website/main.py`
- autorizace uživatele v `./website/auth.py`
- `./website/ajax_views.py` slouží pro asynchroní komunikaci
- `./website/app.py` přiřazuje uživateli roli podle jejich přihlášení
- v `./website/models.py` jsou objekty, díky kterým přistupujeme k databázi implementované v PostgreSQL
- `./website/views.py` se stará o zobrazování souboru html podle toho na co právě uživatel klik
- javascriptové funkce pro AJAX jsou implementovány v souvoru `./website/static/index.js` a zvlášť v jednotlivých html souborech.
- pozadí aplikace je uloženo v `./website/static/background.jpg`
- styly aplikace jsou implementovány v souboru `./website/static/style.css`
- ve složce `./website/templates` je uložená každá stránka aplikace, které jsou propojeny backendem v Python Flask
- ajax_xxxx.html jsou soubory vytvořené, podle stejnojmených souborů, pro asynchronní komunikaci
- 

## Spuštění

### Lokálně

#### Požadavky

- Python
- Flask-SQLAlchemy==2.5.1
- SQLAlchemy==1.3.24
- Flask-Login==0.6.2
- cryptography
- psycopg2
- APScheduler
- Flask-APScheduler

#### Příkaz

Pro spuštění napište tento příkaz v terminálu ve složce `./website`
```
python3 main.py
```

Poté múžete kliknout na odkaz v terminálu nebo do vyhledavače zadat `http://127.0.0.1:5000/`.

