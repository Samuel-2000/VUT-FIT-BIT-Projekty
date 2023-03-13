#autor: Samuel Kuchta (xkucht11)
from app import db
from flask_login import UserMixin
from sqlalchemy.sql import func
#from sqlalchemy.dialects.mysql import LONGTEXT
from sqlalchemy.dialects.postgresql import ARRAY

class Dobrovolnik(db.Model, UserMixin):
    id  = db.Column(db.Integer, primary_key=True)
    email = db.Column(db.String(255), unique=True)
    heslo = db.Column(db.String(255))
    jmeno  = db.Column(db.String(255))
    prijmeni  = db.Column(db.String(255))
    overen = db.Column(db.Boolean, default=False)
    datum_narozeni  = db.Column(db.DateTime(timezone=True)) 
    rozvrh_dobrovolnik = db.Column(ARRAY(db.Integer(), 84))

class Pecovatel(db.Model, UserMixin):
    id  = db.Column(db.Integer, primary_key=True)
    email = db.Column(db.String(255), unique=True)
    heslo = db.Column(db.String(255))
    jmeno  = db.Column(db.String(255))
    prijmeni  = db.Column(db.String(255))
    datum_narozeni  = db.Column(db.DateTime(timezone=True)) 
    adresa  = db.Column(db.String(255))

class Veterinar(db.Model, UserMixin):
    id = db.Column(db.Integer, primary_key=True)
    email = db.Column(db.String(255), unique=True)
    heslo = db.Column(db.String(255))
    jmeno = db.Column(db.String(255))
    prijmeni = db.Column(db.String(255))
    datum_vytvoreni = db.Column(db.DateTime(timezone=True), default=func.now()) 

class Zvire(db.Model):
    id = db.Column(db.Integer, primary_key=True)
    jmeno = db.Column(db.String(255))
    druh = db.Column(db.String(255))
    datum_narozeni = db.Column(db.DateTime(timezone=True)) 
    historie = db.Column(db.String(1023))
    id_pecovatel = db.Column(db.Integer, db.ForeignKey('pecovatel.id'))
    rozvrh_zvire = db.Column(ARRAY(db.Integer(), 84))
    img = db.Column(db.Text, nullable=True) # db.LargeBinary #URL
    mimetype = db.Column(db.Text, nullable=False)
    prirazeno = db.Column(db.Boolean, default=True)

class Pozadavky(db.Model):
    id = db.Column(db.Integer, primary_key=True)
    text = db.Column(db.String(1023))
    id_zvire = db.Column(db.Integer, db.ForeignKey('zvire.id'), nullable=False)
    id_veterinar = db.Column(db.Integer, db.ForeignKey('veterinar.id'))
    vyrizen = db.Column(db.Boolean, default=False)

class Vysetreni(db.Model):
    id = db.Column(db.Integer, primary_key=True)
    druh_vysetreni = db.Column(db.String(31))
    datum = db.Column(db.DateTime(timezone=True), default=func.now()) 
    text = db.Column(db.String(1023))
    id_zvire = db.Column(db.Integer, db.ForeignKey('zvire.id'))

class Admin(db.Model, UserMixin):
    id = db.Column(db.Integer, primary_key=True)
    email = db.Column(db.String(255), unique=True)
    heslo = db.Column(db.String(255))
    jmeno = db.Column(db.String(255))
    prijmeni  = db.Column(db.String(255))

class Vypujcka(db.Model, UserMixin):
    id = db.Column(db.Integer, primary_key=True)
    datum = db.Column(db.DateTime(timezone=True))
    id_dobrovolnik = db.Column(db.Integer, db.ForeignKey('dobrovolnik.id'), nullable=False)
    id_zvire = db.Column(db.Integer, db.ForeignKey('zvire.id'), nullable=False)
    vyrizena = db.Column(db.Boolean, default=False)

class Adopce(db.Model, UserMixin):
    id = db.Column(db.Integer, primary_key=True)
    id_dobrovolnik = db.Column(db.Integer, db.ForeignKey('dobrovolnik.id'), nullable=False)
    id_zvire = db.Column(db.Integer, db.ForeignKey('zvire.id'), nullable=False)
