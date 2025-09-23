from datetime import datetime
from pytz import timezone

JST = timezone("Asia/Tokyo")

def now_jst():
    return datetime.now(JST)