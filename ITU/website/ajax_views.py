"""
autori: Samuel Kuchta (xkucht11)
        Jakub Kováčik (xkovac58)
"""
from flask import Blueprint, render_template, request, flash, jsonify, redirect, url_for
from models import Pecovatel, Veterinar, Zvire, Vysetreni, Pozadavky, Dobrovolnik, Vypujcka
from app import db
from datetime import date
from werkzeug.utils import secure_filename
from base64 import b64encode
from datetime import date, datetime, timedelta
from werkzeug.security import generate_password_hash
from uuid import uuid4
import os
max_int = 65535
from pathlib import Path
import pytz
ViennaTz = pytz.timezone("Europe/Vienna")
from apscheduler.triggers.cron import CronTrigger
from flask import Blueprint, render_template, request, flash, redirect, url_for, session
from models import Dobrovolnik, Pecovatel, Veterinar, Admin, Adopce
from werkzeug.security import generate_password_hash, check_password_hash
from app import db
from flask_login import login_user, login_required, logout_user, current_user
from datetime import date, datetime, timedelta


ajax_views = Blueprint('ajax_views', __name__)

#autor: Jakub Kováčik (xkovac58)
@ajax_views.route('/ajax_animals', methods=['GET'])
def ajax_animals():
    Animals = Zvire.query.all()
    return render_template("ajax_animals.html", user = current_user, animals = Animals)

#autor: Jakub Kováčik (xkovac58)
@ajax_views.route('/ajax_', methods=['GET'])
def ajax_home():
    return render_template("ajax_homepage.html", user = current_user)

#autor: Jakub Kováčik (xkovac58)
@ajax_views.route('/ajax_login', methods=['GET', 'POST'])
def ajax_login():
    if request.method == 'POST':
        email = request.form.get('email')
        password = request.form.get('password')
        
        if (email == "admin@admin.com"):
            if(password == "admin"):
                user = Admin.query.filter_by(email=email).first()
                if not user:
                    user = Admin(email=email, jmeno="Admin", heslo=generate_password_hash(password, method='sha256'))
                    db.session.add(user)
                    db.session.commit()

                login_user(user, remember=True)
                session["user_role"] = "Admin"
                return redirect(url_for('views.admin_home'))
            else:
                flash('Nesprávné heslo správce.', category='error')
                return render_template("ajax_login.html", user=current_user)

        user = Dobrovolnik.query.filter_by(email=email).first()
        if user:
            if (check_password_hash(user.heslo, password)):
                flash('Dobrovolník úspěšně přihlášen.', category='success')
                login_user(user, remember=True)
                session["user_role"] = "Dobrovolnik"
                return redirect(url_for('views.home'))
            else:
                flash('Špatné heslo, zkus znovu.', category='error')
                return render_template("ajax_login.html", user=current_user)

        user = Pecovatel.query.filter_by(email=email).first()
        if user:
            if (check_password_hash(user.heslo, password)):
                flash('Pečovatel úspěšně přihlášen.', category='success')
                login_user(user, remember=True)
                session["user_role"] = "Pecovatel"
                return redirect(url_for('views.home'))
            else:
                flash('Špatné heslo, zkus znovu.', category='error')
                return render_template("ajax_login.html", user=current_user)

        user = Veterinar.query.filter_by(email=email).first()
        if user:
            if (check_password_hash(user.heslo, password)):
                flash('Veterinář úspěšně přihlášen.', category='success')
                login_user(user, remember=True)
                session["user_role"] = "Veterinar"
                return redirect(url_for('views.veterinarian_home'))
            else:
                flash('Špatné heslo, zkus znovu.', category='error')
                return render_template("ajax_login.html", user=current_user)
        else:
            flash('Email neexistuje.', category='error')

    return render_template("ajax_login.html", user=current_user)

#autor: Jakub Kováčik (xkovac58)
@ajax_views.route('/ajax_sign-up', methods=['GET', 'POST'])
def ajax_sign_up():
    if request.method == 'POST':
        email = request.form.get('email')
        first_name = request.form.get('firstName')
        password1 = request.form.get('password1')
        password2 = request.form.get('password2')
        BirthDate = request.form.get('BirthDate')
        input_date = datetime.strptime(BirthDate ,'%Y-%m-%d').date()

        for user in [
            Pecovatel.query.filter_by(email=email).first(),
            Veterinar.query.filter_by(email=email).first(),
            Admin.query.filter_by(email=email).first()]:
            if user:
                flash('Email už existuje.', category='error')
                return render_template("ajax_sign_up.html", user=current_user)

        user = Dobrovolnik.query.filter_by(email=email).first()
        if user:
            flash('Email už existuje.', category='error')
        elif len(email) < 4:
            flash('Email musí obsahovat více než 3 znaky.', category='error')
        elif len(first_name) < 2:
            flash('Křestní jméno musí obsahovat aspoň 1 znak.', category='error')
        elif password1 != password2:
            flash('Hesla se neshodují.', category='error')
        elif len(password1) < 7:
            flash('Heslo musí obsahovat alespoň 7 znaků.', category='error')
        elif input_date > date.today() - timedelta(days=15*365+4):
            flash('Musíte mít alespoň 15 let!', category='error')
        elif input_date < date.today() - timedelta(days=150*365):
            flash('Nesmíte mít víc než 150 let!', category='error')
        else:
            init_rozvrh = []
            for _ in range(0, 84):
                init_rozvrh.append(0)
                
            new_user = Dobrovolnik(email=email, jmeno=first_name, 
                heslo=generate_password_hash(password1, method='sha256'), datum_narozeni=date.fromisoformat(BirthDate), rozvrh_dobrovolnik = init_rozvrh)
            db.session.add(new_user)
            db.session.commit()
            login_user(new_user, remember=True)
            session["user_role"] = "Dobrovolnik"
            flash('Účet vytvořen.', category='success')
            return render_template("ajax_homepage.html", user = current_user)

    return render_template("ajax_sign_up.html", user=current_user)

#autor: Samuel Kuchta (xkucht11)
@ajax_views.route('/ajax_borrowing', methods=['GET'])
def ajax_borrowing():
    Vypujcky = Vypujcka.query.all()
    Animals = Zvire.query.all()
    Volunteers = Dobrovolnik.query.all()
    return render_template("ajax_borrowing.html", user = current_user, vypujcky = Vypujcky, animals = Animals, volunteers = Volunteers)

#autor: Samuel Kuchta (xkucht11)
@ajax_views.route('/ajax_caregiver-home', methods=['GET'])
def ajax_caregiver_home():
    seznam_dobrovolniku = Dobrovolnik.query.all()
    return render_template("ajax_caregiver_home.html", user = current_user, seznam = seznam_dobrovolniku)

#autor: Jakub Kováčik (xkovac58)
@ajax_views.route('/ajax_veterinarian-home', methods=['GET'])
def ajax_veterinarian_home():
    seznam_pozadavku = Pozadavky.query.all()
    veterinar = Veterinar.query.get(current_user.id)
    Animals = Zvire.query.all()
    return render_template("ajax_veterinarian_home.html", user = current_user, seznam = seznam_pozadavku, vet = veterinar, animals = Animals)

#autor: Jakub Kováčik (xkovac58)
@ajax_views.route('/ajax_admin-home', methods=['GET'])
def ajax_admin_home():
    seznam_pecovatelu = Pecovatel.query.all()
    seznam_veterinaru = Veterinar.query.all()
    return render_template("ajax_admin_home.html", user = current_user, pecovatele = seznam_pecovatelu, veterinari = seznam_veterinaru)

#autor: Jakub Kováčik (xkovac58)
@ajax_views.route('/ajax_logout')
@login_required
def ajax_logout():
    logout_user()
    session.pop("user_role", None)
    return redirect(url_for('ajax_views.ajax_login'))

#autor: Samuel Kuchta (xkucht11)
@ajax_views.route('/ajax_caregiver-add', methods=['GET', 'POST'])
def ajax_caregiver_add():
    if request.method == 'POST':
        email = request.form.get('email')
        first_name = request.form.get('firstName')
        password1 = request.form.get('password1')
        password2 = request.form.get('password2')
        BirthDate = request.form.get('BirthDate')
        input_date = datetime.strptime(BirthDate ,'%Y-%m-%d').date()

        for user in [
            Dobrovolnik.query.filter_by(email=email).first(),
            Veterinar.query.filter_by(email=email).first(),
            Admin.query.filter_by(email=email).first()]:
            if user:
                flash('Email už existuje.', category='error')
                return render_template("ajax_caregiver_add.html", user=current_user)

        user = Pecovatel.query.filter_by(email=email).first()
        if user:
            flash('Email already exists.', category='error')
        elif len(email) < 4:
            flash('Email musí obsahovat více než 3 znaky.', category='error')
        elif len(first_name) < 2:
            flash('Křestní jméno musí obsahovat aspoň 1 znak.', category='error')
        elif password1 != password2:
            flash('Hesla se neshodují.', category='error')
        elif len(password1) < 7:
            flash('Heslo musí obsahovat alespoň 7 znaků.', category='error')
        elif datetime.strptime(BirthDate ,'%Y-%m-%d').date() > date.today() - timedelta(days=18*365+4):
            flash('Musíte mít alespoň 18 let!', category='error')
        elif input_date < date.today() - timedelta(days=150*365):
            flash('Pecovatel nesmí mít víc než 150 let!', category='error')
        else:
            new_user = Pecovatel(email=email, jmeno=first_name, heslo=generate_password_hash(password1, method='sha256'), datum_narozeni=date.fromisoformat(BirthDate))
            db.session.add(new_user)
            db.session.commit()

            flash('Účet vytvořen.', category='success')
            seznam_pecovatelu = Pecovatel.query.all()
            seznam_veterinaru = Veterinar.query.all()
            return render_template("ajax_admin_home.html", user = current_user, pecovatele = seznam_pecovatelu, veterinari = seznam_veterinaru)

    return render_template("ajax_caregiver_add.html", user=current_user)

#autor: Jakub Kováčik (xkovac58)
@ajax_views.route('/ajax_veterinarian-add', methods=['GET', 'POST'])
def ajax_veterinarian_add():
    if request.method == 'POST':
        email = request.form.get('email')
        first_name = request.form.get('firstName')
        password1 = request.form.get('password1')
        password2 = request.form.get('password2')

        for user in [
            Dobrovolnik.query.filter_by(email=email).first(),
            Pecovatel.query.filter_by(email=email).first(),
            Admin.query.filter_by(email=email).first()]:
            if user:
                flash('Email už existuje.', category='error')
                return render_template("ajax_veterinarian_add.html", user=current_user)

        user = Veterinar.query.filter_by(email=email).first()
        if user:
            flash('Email už existuje.', category='error')
        elif len(email) < 4:
            flash('Email musí obsahovat více než 3 znaky.', category='error')
        elif len(first_name) < 2:
            flash('Křestní jméno musí obsahovat aspoň 1 znak.', category='error')
        elif password1 != password2:
            flash('Hesla se neshodují.', category='error')
        elif len(password1) < 7:
            flash('Heslo musí obsahovat alespoň 7 znaků.', category='error')
        else:
            new_user = Veterinar(email=email, jmeno=first_name, heslo=generate_password_hash(password1, method='sha256'))
            db.session.add(new_user)
            db.session.commit()

            flash('Účet vytvořen.', category='success')
            seznam_pecovatelu = Pecovatel.query.all()
            seznam_veterinaru = Veterinar.query.all()
            return render_template("ajax_admin_home.html", user = current_user, pecovatele = seznam_pecovatelu, veterinari = seznam_veterinaru)

    return render_template("ajax_veterinarian_add.html", user=current_user)

#autor: Jakub Kováčik (xkovac58)
@ajax_views.route('/ajax_animal-add', methods=['GET', 'POST'])
def ajax_animal_add():
    if request.method == 'POST':
        jmeno = request.form.get('Name')
        druh = request.form.get('Breed')
        BirthDate = request.form.get('BirthDate')
        historie = request.form.get('History')

        if len(jmeno) < 2:
            flash('Křestní jméno musí být delší jak 1 znak.', category='error')
        elif len(druh) < 3:
            flash('Plemeno musí být delší jak 2 znaky.', category='error')
        elif datetime.strptime(BirthDate ,'%Y-%m-%d').date() > date.today():
            flash('Datum narození nesmí být z budoucnosti', category='error')
        else:
            if not historie:
                historie = "Žádná historie."
            init_rozvrh = []
            for _ in range(0, 84):
                init_rozvrh.append(max_int)

            new_animal = Zvire(jmeno = jmeno, druh = druh, datum_narozeni = date.fromisoformat(BirthDate), historie = historie, id_pecovatel = current_user.id, rozvrh_zvire = init_rozvrh, img = '', mimetype = '')

            db.session.add(new_animal)
            db.session.commit()
            flash('Zvíře přidáno', category='success')

            return render_template("ajax_animal_change.html", user = current_user, animal = new_animal)

    return render_template("ajax_animal_add.html", user = current_user)

#autor: Samuel Kuchta (xkucht11)
@ajax_views.route('/ajax_caregiver-about-<animal_id>', methods=['GET', 'POST'])
def ajax_caregiver_about(animal_id):
    changes = 0
    Caregiver = Pecovatel.query.get(animal_id)
    if request.method == 'POST':
        email = request.form.get('email')
        first_name = request.form.get('firstName')
        password1 = request.form.get('password1')
        password2 = request.form.get('password2')
        BirthDate = request.form.get('BirthDate')

        if email:
            user = Dobrovolnik.query.filter_by(email = email).first()
            if user:
                flash('Email už existuje.', category='error')
            elif len(email) < 4:
                flash('Email musí být delší jak 3 znaky.', category='error')
            else:
                Caregiver.email = email
                changes = 1

        if first_name:
            if len(first_name) < 2:
                flash('Křestní jméno musí být delší jak 1 znak.', category='error')
            else:
                Caregiver.jmeno = first_name
                changes = 1

        if password1 and password2:
            if password1 != password2:
                flash('Hesla se neshodují.', category='error')
            elif len(password1) < 7:
                flash('Heslo musí obsahovat alespoň 7 znaků.', category='error')
            else:
                Caregiver.heslo = generate_password_hash(password1, method='sha256')
                changes = 1

        if BirthDate:
            input_date = datetime.strptime(BirthDate ,'%Y-%m-%d').date()
            if input_date > date.today() - timedelta(days=18*365+4):
                flash('Pečovatel musí mít alespoň 18 let!', category='error')
            elif input_date < date.today() - timedelta(days=150*365):
                flash('Pečovatel nesmí mít víc než 150 let!', category='error')
            else:
                Caregiver.datum_narozeni=date.fromisoformat(BirthDate)
                changes = 1

        if changes == 1:    
            db.session.commit()
            flash('Údaje změneny.', category='success')
        else: 
            flash('Údaje nezměneny.', category='error')

        seznam_pecovatelu = Pecovatel.query.all()
        seznam_veterinaru = Veterinar.query.all()
        return render_template("ajax_admin_home.html", user = current_user, pecovatele = seznam_pecovatelu, veterinari = seznam_veterinaru)

    return render_template("ajax_caregiver_about.html", user = current_user, caregiver = Caregiver)

#autor: Jakub Kováčik (xkovac58)
@ajax_views.route('/ajax_animal-<animal_id>', methods=['GET'])
def ajax_animal(animal_id):
    Animal = Zvire.query.get(animal_id)
    date = Animal.datum_narozeni.strftime("%d. %b. %Y")
    Vysetreni_zvirete = Vysetreni.query.filter_by(id_zvire = animal_id)
    pocet_vysetreni = Vysetreni_zvirete.count()
    return render_template("ajax_animal.html", user = current_user, animal = Animal, kniha = Vysetreni_zvirete, kniha_len = pocet_vysetreni, date = date)

#autor: Jakub Kováčik (xkovac58)
@ajax_views.route('/ajax_veterinarian-about-<animal_id>', methods=['GET', 'POST'])
def ajax_veterinarian_about(animal_id):
    changes = 0
    Veterinarian = Veterinar.query.get(animal_id)
    if request.method == 'POST':
        email = request.form.get('email')
        first_name = request.form.get('firstName')
        password1 = request.form.get('password1')
        password2 = request.form.get('password2')

        if email:
            user = Dobrovolnik.query.filter_by(email = email).first()
            if user:
                flash('Email už existuje.', category='error')
            elif len(email) < 4:
                flash('Email musí být delší jak 3 znaky.', category='error')
            else:
                Veterinarian.email = email
                changes = 1

        if first_name:
            if len(first_name) < 2:
                flash('Křestní jméno musí být delší jak 1 znak.', category='error')
            else:
                Veterinarian.jmeno = first_name
                changes = 1

        if password1 and password2:
                if password1 != password2:
                    flash('Hesla se neshodují.', category='error')
                elif len(password1) < 7:
                    flash('Heslo musí obsahovat alespoň 7 znaků.', category='error')
                else:
                    Veterinarian.heslo = generate_password_hash(password1, method='sha256')
                    changes = 1

        if changes == 1:
            db.session.commit()
            flash('Údaje změneny.', category='success')
        else:
            flash('Údaje nezměneny.', category='error')
        
        seznam_pecovatelu = Pecovatel.query.all()
        seznam_veterinaru = Veterinar.query.all()
        return render_template("ajax_admin_home.html", user = current_user, pecovatele = seznam_pecovatelu, veterinari = seznam_veterinaru)

    return render_template("ajax_veterinarian_about.html", user = current_user, veterinarian = Veterinarian)

#autor: Samuel Kuchta (xkucht11)
@ajax_views.route('/ajax_animal-outings-register-<animal_id>', methods=['GET', 'POST'])
def ajax_animal_outings_register(animal_id):
    table_builder = zip([0, 1, 2, 3, 4, 5, 6], ["Po", "Út", "St",  "Čt", "Pá", "So", "Ne"])
    Animal = Zvire.query.filter_by(id = animal_id).first()

    now = datetime.now(ViennaTz)
    day = now.weekday()
    hour = int(now.strftime("%H"))
    if 8 <= hour <=19:
        cell_id = day*12+hour-8
    else:
        cell_id = 84
    if request.method == 'POST':
        int_list_zvire = []
        int_list_dobrovolnik = []
        for i in range(0, 84):
            
            checkbox_id = request.form.get(f'{i}')
            checked_day = i//12
            checked_hour = i+8-checked_day*12
            delta_days = checked_day - day
            if delta_days < 0:
                delta_days = delta_days + 7
            if delta_days == 0 and checked_hour < hour:
                delta_days = delta_days +7
            #final_date = (datetime.now() + timedelta(days=delta_days)).replace(hour=checked_hour , minute=0 , second=0 ,microsecond=0)
            
            
            if checkbox_id == "true":  # ak je zaskrtnuty checkbox, 
                int_list_zvire.append(current_user.id)  # ulozi id dobrovolnika do tabulky zvierata
                int_list_dobrovolnik.append(animal_id)  # ulozi id zvierata do tabulky dobrovolnika
                """
                borrowing = Vypujcka.query.filter_by(datum = final_date , id_dobrovolnik = current_user.id , id_zvire = animal_id).first()
                if not borrowing:
                    new_vypujcka = Vypujcka(datum = final_date , id_dobrovolnik = current_user.id , id_zvire = animal_id)
                    db.session.add(new_vypujcka)
                """
            elif checkbox_id == "false":  # nezaskrtnuty
                
                int_list_zvire.append(0)
                int_list_dobrovolnik.append(0)
            else:  
                int_list_zvire.append(Animal.rozvrh_zvire[i])
                int_list_dobrovolnik.append(current_user.rozvrh_dobrovolnik[i])
                

        animal_list = list(Animal.rozvrh_zvire)
        dobrovolnik_list = list(current_user.rozvrh_dobrovolnik)

        for i in range(0, 84):
            animal_list[i] = int_list_zvire[i]
            dobrovolnik_list[i] = int_list_dobrovolnik[i]
            
        Animal.rozvrh_zvire = tuple(animal_list)
        current_user.rozvrh_dobrovolnik = tuple(dobrovolnik_list)
        db.session.commit()

    is_registered_list = []
    for i in range(0, 84):
        is_registered_list.append(Animal.rozvrh_zvire[i])

    return render_template("ajax_animal_outings_register.html", user = current_user, animal = Animal, is_registered = is_registered_list, table_build = table_builder, max = max_int, min = 0, usr_id_chr = current_user.id, cellid = cell_id)

#autor: Samuel Kuchta (xkucht11)
@ajax_views.route('/ajax_examination-requests-<animal_id>', methods=['GET', 'POST'])
def ajax_examination_requests(animal_id):
    Veterinarians = Veterinar.query.all()
    Animal = Zvire.query.get(animal_id)
    seznam_pozadavku = Pozadavky.query.filter_by(id_zvire = Animal.id)
    if request.method == 'POST':
        vet_id = request.form.get('vet_id')
        text = request.form.get('text')
        if len(text) < 4:
            flash('Popis musí obsahovat alespoň 4 znaky', category='error')
        else:
            veterinar = Veterinar.query.get(vet_id)
            new_pozadavek = Pozadavky(text = text, id_zvire = animal_id, id_veterinar = veterinar.id)
            db.session.add(new_pozadavek)
            db.session.commit()
            flash('Požadavek vytvořen', category='success')
            Animal = Zvire.query.get(animal_id)
            date = Animal.datum_narozeni.strftime("%d. %b. %Y")
            Vysetreni_zvirete = Vysetreni.query.filter_by(id_zvire = animal_id)
            pocet_vysetreni = Vysetreni_zvirete.count()
            return render_template("ajax_animal.html", user = current_user, animal = Animal, kniha = Vysetreni_zvirete, kniha_len = pocet_vysetreni, date = date)

    return render_template("ajax_examination_requests.html", user = current_user, id_zvire = animal_id, animal = Animal, seznam = seznam_pozadavku, veterinarians = Veterinarians)

#autor: Jakub Kováčik (xkovac58)
@ajax_views.route('/ajax_animal-change-<animal_id>', methods=['GET', 'POST'])
def ajax_animal_change(animal_id):
    changes = 0
    Animal = Zvire.query.get(animal_id)
    if request.method == 'POST':        
        jmeno = request.form.get('Name')
        druh = request.form.get('Breed')
        BirthDate = request.form.get('BirthDate')
        historie = request.form.get('History')
        img = False
        try:
            img = request.files['img']
        except:
            pass

        if jmeno:
            if len(jmeno) < 2:
                flash('Křestní jméno musí být delší jak 1 znak.', category='error')
            else:
                Animal.jmeno = jmeno
                changes = 1

        if druh:
            if len(druh) < 3:
                flash('Plemeno musí být delší jak 2 znaky.', category='error')
            else:
                Animal.druh = druh
                changes = 1

        if BirthDate:
            if datetime.strptime(BirthDate ,'%Y-%m-%d').date() > date.today():
                flash('Datum narození nesmí být z budoucnosti', category='error')
            else:
                Animal.datum_narozeni = date.fromisoformat(BirthDate)
                changes = 1

        if historie:
            Animal.historie = historie
            changes = 1

        if img != False:
            filename = secure_filename(img.filename)
            uniq_img = make_unique(filename)
            dirname = os.path.dirname(__file__)
            dirname2 = Path(dirname, "static")
            img.save(os.path.join(dirname2, uniq_img))
            mimetype = img.mimetype
            if not filename or not mimetype:
                flash('Špatně nahraný nebo žádný obrázek.', category='error')
            Animal.img = uniq_img
            Animal.mimetype = mimetype
            changes = 1

        if changes == 1:
            db.session.commit()
            flash('Zvíře změněno.', category='success')
            return render_template("ajax_animal_change.html", user = current_user, animal = Animal)
        else: 
            flash('Údaje nebyly změneny.', category='error')

    return render_template("ajax_animal_change.html", user = current_user, animal = Animal)

#autor: Jakub Kováčik (xkovac58)
@ajax_views.route('/ajax_animal-delete-<animal_id>', methods=['GET'])
def ajax_animal_delete(animal_id):
    Vysetreni.query.filter_by(id_zvire = animal_id).delete()
    Vypujcka.query.filter_by(id_zvire = animal_id).delete()
    Pozadavky.query.filter_by(id_zvire = animal_id).delete()
    Zvire.query.filter_by(id = animal_id).delete()
    db.session.commit()
    flash('Zvíře odebráno.', category='success')
    Animals = Zvire.query.all()
    return render_template("ajax_animals.html", user = current_user, animals = Animals)

#autor: Jakub Kováčik (xkovac58)
@ajax_views.route('/ajax_record-show-<animal_id>-<exam_id>', methods=['GET'])
def ajax_record_show(animal_id, exam_id):
    Zaznam = Vysetreni.query.get(exam_id)
    Animal = Zvire.query.get(animal_id)
    return render_template("ajax_record_show.html", user = current_user, record = Zaznam, animal = Animal)

#autor: Jakub Kováčik (xkovac58)
@ajax_views.route('/ajax_record-add-<animal_id>', methods=['GET', 'POST'])
def ajax_record_add(animal_id):
    Zaznam = Vysetreni.query.get(animal_id)
    Animal = Zvire.query.get(animal_id)
    seznam_pozadavku = Pozadavky.query.filter_by(id_zvire = animal_id)
    is_post = False
    if request.method == 'POST':
        is_post = True
        type = request.form.get('druh_vysetreni')
        Record_date = request.form.get('datum')
        text = request.form.get('text')
        req_id = request.form.get('req_id')

        if len(type) < 1:
            flash('Název musí mít aspoň 1 znak.', category='error')
        elif len(text) < 4:
            flash('Popis musí mít alespoň 4 znaky.', category='error')
        elif datetime.strptime(Record_date ,'%Y-%m-%d').date() > date.today():
            flash('Datum nesmí být z budoucnosti', category='error')
        else:
            new_record = Vysetreni(druh_vysetreni = type, datum = date.fromisoformat(Record_date), text = text, id_zvire = animal_id)
            if req_id != "Blank":
                ajax_request_delete(req_id)
            db.session.add(new_record)
            db.session.commit()
            flash('Vyšetření přidáno.', category='success')
            seznam_pozadavku = Pozadavky.query.all()
            veterinar = Veterinar.query.get(current_user.id)
            Animals = Zvire.query.all()
            return render_template("ajax_veterinarian_home.html", user = current_user, seznam = seznam_pozadavku, vet = veterinar, animals = Animals)

    return render_template("ajax_record_add.html", user = current_user, id_zvire = animal_id, record = Zaznam, animal = Animal, requests = seznam_pozadavku, is_post = is_post)

#autor: Samuel Kuchta (xkucht11)
@ajax_views.route('/ajax_request-delete-<request_id>', methods=['GET'])
def ajax_request_delete(request_id):
    Request = Pozadavky.query.filter_by(id = request_id).first()
    Request.vyrizen = True
    db.session.commit()
    flash('Žádost o vyšetření byla smazána.', category='success')
    seznam_pozadavku = Pozadavky.query.all()
    veterinar = Veterinar.query.get(current_user.id)
    Animals = Zvire.query.all()
    return render_template("ajax_veterinarian_home.html", user = current_user, seznam = seznam_pozadavku, vet = veterinar, animals = Animals)

#autor: Samuel Kuchta (xkucht11)
@ajax_views.route('/ajax_animal-outings-change-<animal_id>', methods=['GET', 'POST'])
def ajax_animal_outings_change(animal_id):
    table_builder = zip([0, 1, 2, 3, 4, 5, 6], ["Po", "Út", "St",  "Čt", "Pá", "So", "Ne"])
    Animal = Zvire.query.filter_by(id = animal_id).first()

    now = datetime.now(ViennaTz)
    day = now.weekday()
    hour = int(now.strftime("%H"))
    if 8 <= hour <=19:
        cell_id = day*12+hour-8
    else:
        cell_id = 84

    if request.method == 'POST':
        int_list_zvire = []
        for i in range(0, 84):
            checkbox_id = request.form.get(f'{i}')
            if checkbox_id == "true":  # ak je zaskrtnuty checkbox,
                if Animal.rozvrh_zvire[i] != max_int:  # ak uz bol otvoreny a niekto uz registroval
                    int_list_zvire.append(Animal.rozvrh_zvire[i])  # nastavi termin pre zviera ako obsadeny
                else:
                    int_list_zvire.append(0)  # nastavi termin pre zviera ako otvoreny
            else:  # ak je prazdny checkbox
                int_list_zvire.append(max_int)  # nastavi termin pre zviera ako zavrety
                # TODO, ak niekto mal registrovany termin, tak odregistrovat u rozvrhu uzivatela.

        animal_list = list(Animal.rozvrh_zvire)
        for i in range(0, 84):
            animal_list[i] = int_list_zvire[i]

        Animal.rozvrh_zvire = tuple(animal_list)
        db.session.commit()

    is_registered_list = []
    for i in range(0, 84):
        is_registered_list.append(Animal.rozvrh_zvire[i])

    return render_template("ajax_animal_outings_change.html", user = current_user, animal = Animal, is_registered = is_registered_list, table_build = table_builder, max = max_int, min = 0, cellid = cell_id)

#autor: Jakub Kováčik (xkovac58)
@ajax_views.route('/ajax_caregiver-delete-<caregiver_id>', methods=['GET'])
def ajax_caregiver_delete(caregiver_id):
    seznam_zvirat = Zvire.query.filter_by(id_pecovatel = caregiver_id)
    for zvire in seznam_zvirat:
        zvire.prirazeno = False
        zvire.id_pecovatel = None
    Pecovatel().query.filter_by(id = caregiver_id).delete()
    db.session.commit()
    flash('Pečovatel odebrán.', category='success')
    seznam_pecovatelu = Pecovatel.query.all()
    seznam_veterinaru = Veterinar.query.all()
    return render_template("ajax_admin_home.html", user = current_user, pecovatele = seznam_pecovatelu, veterinari = seznam_veterinaru)

#autor: Jakub Kováčik (xkovac58)
@ajax_views.route('/ajax_veterinarian-delete-<veterinarian_id>', methods=['GET'])
def ajax_veterinarian_delete(veterinarian_id):
    Pozadavky.query.filter_by(id_veterinar = veterinarian_id).delete()
    Veterinar.query.filter_by(id = veterinarian_id).delete()
    db.session.commit()
    flash('Veterinár odebrán.', category='success')
    seznam_pecovatelu = Pecovatel.query.all()
    seznam_veterinaru = Veterinar.query.all()
    return render_template("ajax_admin_home.html", user = current_user, pecovatele = seznam_pecovatelu, veterinari = seznam_veterinaru)

#autor: Samuel Kuchta (xkucht11)
@ajax_views.route('/ajax_volunteer-about-<volunteer_id>', methods=['GET', 'POST'])
def ajax_volunteer_about(volunteer_id):
    Volunteer = Dobrovolnik.query.get(volunteer_id)
    Animals = Zvire.query.all()
    seznam_Borrowing = Vypujcka.query.filter_by(id_dobrovolnik = current_user.id)
    
    table_builder = zip([0, 1, 2, 3, 4, 5, 6], ["Po", "Út", "St",  "Čt", "Pá", "So", "Ne"])
    is_registered_list = []
    for i in range(0, 84):
        is_registered_list.append(Volunteer.rozvrh_dobrovolnik[i])

    now = datetime.now(ViennaTz)
    day = now.weekday()
    hour = int(now.strftime("%H"))
    if 8 <= hour <=19:
        cell_id = day*12+hour-8
    else:
        cell_id = 84

    if request.method == 'POST':
        overen = request.form.get('overen')
        if overen == "True":
            Volunteer.overen = True
        else:
            Volunteer.overen = False
        db.session.commit()
        flash('Dobrovolník změnen.', category='success')
        return render_template("ajax_volunteer_about.html", user = current_user, volunteer = Volunteer, is_registered = is_registered_list, table_build = table_builder, min = 0, animals = Animals, cellid = cell_id , borrowings = seznam_Borrowing)


    return render_template("ajax_volunteer_about.html", user = current_user, volunteer = Volunteer, is_registered = is_registered_list, table_build = table_builder, min = 0, animals = Animals, cellid = cell_id , borrowings = seznam_Borrowing)

#autor: Samuel Kuchta (xkucht11)
@ajax_views.route('/ajax_volunteer-home-<volunteer_id>', methods=['GET', 'POST'])
def ajax_volunteer_home(volunteer_id):
    Animals = Zvire.query.all()
    Volunteer = Dobrovolnik.query.get(volunteer_id)
    seznam_Borrowing = Vypujcka.query.filter_by(id_dobrovolnik = current_user.id)
    
    table_builder = zip([0, 1, 2, 3, 4, 5, 6], ["Po", "Út", "St",  "Čt", "Pá", "So", "Ne"])
    is_registered_list = []
    for i in range(0, 84):
        is_registered_list.append(Volunteer.rozvrh_dobrovolnik[i])

    now = datetime.now(ViennaTz)
    day = now.weekday()
    hour = int(now.strftime("%H"))
    if 8 <= hour <=19:
        cell_id = day*12+hour-8
    else:
        cell_id = 84

    if request.method == 'POST':
        changes = 0
        email = request.form.get('email')
        first_name = request.form.get('firstName')
        password1 = request.form.get('password1')
        password2 = request.form.get('password2')
        BirthDate = request.form.get('BirthDate')

        if email:
            user = Dobrovolnik.query.filter_by(email = email).first()
            if user:
                flash('Email už existuje.', category='error')
            elif len(email) < 4:
                flash('Email musí být delší jak 3 znaky.', category='error')
            else:
                Volunteer.email = email
                Volunteer.overen = False
                changes = 1

        if first_name:
            if len(first_name) < 2:
                flash('Křestní jméno musí být delší jak 1 znak.', category='error')
            else:
                Volunteer.jmeno = first_name
                Volunteer.overen = False
                changes = 1

        if password1 and password2:
                if password1 != password2:
                    flash('Hesla se neshodují.', category='error')
                elif len(password1) < 7:
                    flash('Heslo musí obsahovat alespoň 7 znaků.', category='error')
                else:
                    Volunteer.heslo = generate_password_hash(password1, method='sha256')
                    changes = 1
        if BirthDate:
            input_date = datetime.strptime(BirthDate ,'%Y-%m-%d').date()
            if input_date > date.today() - timedelta(days=15*365+4):
                flash('Musíte mít alespoň 15 let!', category='error')
            elif input_date < date.today() - timedelta(days=150*365):
                flash('Nesmíte mít víc než 150 let!', category='error')
            else:
                Volunteer.datum_narozeni=date.fromisoformat(BirthDate)
                changes = 1

        if changes == 1:
            db.session.commit()
            flash('Údaje změneny, bude třeba ověřit totožnost.', category='success')
            return render_template("ajax_volunteer_home.html", user = current_user, volunteer = Volunteer, is_registered = is_registered_list, table_build = table_builder, min = 0, animals = Animals, cellid = cell_id, borrowings = seznam_Borrowing)
        else: 
            flash('Údaje nezměneny.', category='error')

    return render_template("ajax_volunteer_home.html", user = current_user, volunteer = Volunteer, is_registered = is_registered_list, table_build = table_builder, min = 0, animals = Animals, cellid = cell_id, borrowings = seznam_Borrowing)

#autor: Samuel Kuchta (xkucht11)
@ajax_views.route('/ajax_vyrizen-<vypujcka_id>', methods=['GET'])
def ajax_vyrizen(vypujcka_id):
    vypujcka = Vypujcka.query.get(vypujcka_id)
    vypujcka.vyrizena = True
    db.session.commit()
    flash('Výpujčka byla ověřena.', category='success')
    Vypujcky = Vypujcka.query.all()
    Animals = Zvire.query.all()
    Volunteers = Dobrovolnik.query.all()
    return render_template("ajax_borrowing.html", user = current_user, vypujcky = Vypujcky, animals = Animals, volunteers = Volunteers)

#autor: Samuel Kuchta (xkucht11)
@ajax_views.route('/ajax_animal-take-<animal_id>-<pecovatel_id>', methods=['GET'])
def ajax_animal_take(animal_id , pecovatel_id):
    Animal = Zvire.query.filter_by(id = animal_id).first()
    Animal.id_pecovatel = pecovatel_id
    Animal.prirazeno = True
    db.session.commit()
    flash('Zvíře přivlastneno.', category='success')
    Animal = Zvire.query.get(animal_id)
    date = Animal.datum_narozeni.strftime("%d. %b. %Y")
    Vysetreni_zvirete = Vysetreni.query.filter_by(id_zvire = animal_id)
    pocet_vysetreni = Vysetreni_zvirete.count()
    return render_template("ajax_animal.html", user = current_user, animal = Animal, kniha = Vysetreni_zvirete, kniha_len = pocet_vysetreni, date = date)

#autor: Jakub Kováčik (xkovac58)
@ajax_views.route('/ajax_animal_filter-<breed>-<name>', methods=['GET'])
def ajax_animal_filter(breed, name):
    if breed != "Všechny" and name != "Všechny":
        Animals = Zvire.query.filter_by(jmeno = name, druh = breed)

    elif breed != "Všechny" and name == "Všechny":
        Animals = Zvire.query.filter_by(druh = breed)

    elif breed == "Všechny" and name != "Všechny":
        Animals = Zvire.query.filter_by(jmeno = name) 
    else:
        Animals = Zvire.query.all()

    return render_template("filtered_animals.html", user = current_user, animals = Animals)

#autor: Jakub Kováčik (xkovac58)
@ajax_views.route('/ajax_animal-adopt-<animal_id>-<dobrovolnik_id>', methods=['GET'])
def ajax_animal_adopt(animal_id , dobrovolnik_id):
    new_adoption = Adopce(id_dobrovolnik = dobrovolnik_id, id_zvire = animal_id)
    db.session.add(new_adoption)
    db.session.commit()
    flash('žádost o adopci byla poslána.', category='success')
    Animal = Zvire.query.get(animal_id)
    date = Animal.datum_narozeni.strftime("%d. %b. %Y")
    Vysetreni_zvirete = Vysetreni.query.filter_by(id_zvire = animal_id)
    pocet_vysetreni = Vysetreni_zvirete.count()
    return render_template("ajax_animal.html", user = current_user, animal = Animal, kniha = Vysetreni_zvirete, kniha_len = pocet_vysetreni, date = date)

#autor: Jakub Kováčik (xkovac58)
@ajax_views.route('/ajax_adoptions', methods=['GET'])
def ajax_adoptions():
    Adoptions = Adopce.query.all()
    Animals = Zvire.query.all()
    Volunteers = Dobrovolnik.query.all()
    return render_template("ajax_adoptions.html", user = current_user, adoptions = Adoptions, animals = Animals, volunteers = Volunteers)

#autor: Jakub Kováčik (xkovac58)
@ajax_views.route('/ajax_approve_adoption-<adopce_id>', methods=['GET'])
def approve_adoption(adopce_id):
    Adopce.query.filter_by(id = adopce_id).delete()
    Adoptions = Adopce.query.all()
    Animals = Zvire.query.all()
    Volunteers = Dobrovolnik.query.all()
    return render_template("ajax_adoptions.html", user = current_user, adoptions = Adoptions, animals = Animals, volunteers = Volunteers)

def make_unique(string):
    ident = uuid4().__str__()
    return f"{ident}-{string}"