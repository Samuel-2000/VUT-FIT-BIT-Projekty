from app import create_app
import atexit
from apscheduler.triggers.cron import CronTrigger
from apscheduler.schedulers.background import BackgroundScheduler
from views import rozvrh_vypujcka_update

app = create_app()

if __name__ == '__main__':
    scheduler = BackgroundScheduler()
    trigger = CronTrigger(year="*", month="*", day="*", hour="*", minute="0", second="0") # mozno hour 8-20

    scheduler.add_job(rozvrh_vypujcka_update, trigger=trigger)
    scheduler.start()

    atexit.register(lambda: scheduler.shutdown())

    app.run(debug=False, host="0.0.0.0")


