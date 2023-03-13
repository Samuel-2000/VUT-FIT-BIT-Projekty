#autor: Samuel Kuchta (xkucht11)
from flask import Flask, session
from flask_sqlalchemy import SQLAlchemy
from os import path
from flask_login import LoginManager
from datetime import timedelta

db = SQLAlchemy()


def create_app():
    app = Flask(__name__)
    app.config['SECRET_KEY'] = 'some secret key'
    app.config['SQLALCHEMY_DATABASE_URI'] = 'postgres link'
    app.config['SQLALCHEMY_TRACK_MODIFICATIONS'] = False
    db.init_app(app)

    from views import views
    from ajax_views import ajax_views
    from auth import auth

    app.register_blueprint(views, url_prefix='/')
    app.register_blueprint(ajax_views, url_prefix='/')
    app.register_blueprint(auth, url_prefix='/')

    from models import Dobrovolnik, Pecovatel, Veterinar, Admin

    create_database(app)

    login_manager = LoginManager()
    login_manager.login_view = 'auth.login'
    login_manager.init_app(app)

    @login_manager.user_loader
    def load_user(id):
        session.permanent = True
        app.permanent_session_lifetime = timedelta(minutes=120)
        if session["user_role"] == "Admin":
            return Admin.query.get(int(id))
        elif session["user_role"] == "Dobrovolnik":
            return Dobrovolnik.query.get(int(id))
        elif session["user_role"] == "Pecovatel":
            return Pecovatel.query.get(int(id))
        elif session["user_role"] == "Veterinar":
            return Veterinar.query.get(int(id))
        else:
            pass

    return app


def create_database(app):
    if not path.exists('website/utulek_db2'):
        db.create_all(app=app)
        print('Created Database!')
