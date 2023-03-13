from flask import Blueprint, render_template, request, flash, redirect, url_for, session
from models import Dobrovolnik, Pecovatel, Veterinar, Admin
from werkzeug.security import generate_password_hash, check_password_hash
from app import db
from flask_login import login_user, login_required, logout_user, current_user
from datetime import date, datetime, timedelta


auth = Blueprint('auth', __name__)


@auth.route('/login', methods=['GET', 'POST'])
def login():
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
                return render_template("login.html", user=current_user)

        user = Dobrovolnik.query.filter_by(email=email).first()
        if user:
            if (check_password_hash(user.heslo, password)):
                flash('Dobrovolník úspěšně přihlášen.', category='success')
                login_user(user, remember=True)
                session["user_role"] = "Dobrovolnik"
                return redirect(url_for('views.home'))
            else:
                flash('Špatné heslo, zkus znovu.', category='error')
                return render_template("login.html", user=current_user)

        user = Pecovatel.query.filter_by(email=email).first()
        if user:
            if (check_password_hash(user.heslo, password)):
                flash('Pečovatel úspěšně přihlášen.', category='success')
                login_user(user, remember=True)
                session["user_role"] = "Pecovatel"
                return redirect(url_for('views.home'))
            else:
                flash('Špatné heslo, zkus znovu.', category='error')
                return render_template("login.html", user=current_user)

        user = Veterinar.query.filter_by(email=email).first()
        if user:
            if (check_password_hash(user.heslo, password)):
                flash('Veterinář úspěšně přihlášen.', category='success')
                login_user(user, remember=True)
                session["user_role"] = "Veterinar"
                return redirect(url_for('views.veterinarian_home'))
            else:
                flash('Špatné heslo, zkus znovu.', category='error')
                return render_template("login.html", user=current_user)
        else:
            flash('Email neexistuje.', category='error')

    return render_template("login.html", user=current_user)


@auth.route('/logout')
@login_required
def logout():
    logout_user()
    session.pop("user_role", None)
    return redirect(url_for('auth.login'))


@auth.route('/sign-up', methods=['GET', 'POST'])
def sign_up():
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
                return render_template("sign_up.html", user=current_user)

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
            return redirect(url_for('views.home'))

    return render_template("sign_up.html", user=current_user)


@auth.route('/caregiver-add', methods=['GET', 'POST'])
def caregiver_add():
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
                return render_template("caregiver_add.html", user=current_user)

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
            return redirect(url_for('views.admin_home'))

    return render_template("caregiver_add.html", user=current_user)


@auth.route('/veterinarian-add', methods=['GET', 'POST'])
def veterinarian_add():
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
                return render_template("veterinarian_add.html", user=current_user)

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
            return redirect(url_for('views.admin_home'))

    return render_template("veterinarian_add.html", user=current_user)
