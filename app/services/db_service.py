from sqlalchemy.orm import Session
from app import models, database

def save_message(db: Session, user_id: str, text: str):
    """メッセージを保存する"""
    message = models.Message(user_id=user_id, text=text)
    db.add(message)
    db.commit()
    db.refresh(message)
    return message

def save_curfew(db: Session, user_id: str, time_str: str):
    """門限を保存する"""
    curfew = db.query(models.Curfew).filter(models.Curfew.user_id == user_id).first()
    if curfew:
        curfew.time = time_str
    else:
        curfew = models.Curfew(user_id=user_id, time=time_str)
        db.add(curfew)
    db.commit()
    db.refresh(curfew)
    return curfew

def get_curfew(user_id: str) -> str | None:
    """門限を取得する"""
    with Session(database.engine) as session:
        curfew = session.query(models.Curfew).filter_by(user_id=user_id).first()
        return curfew.time if curfew else None
