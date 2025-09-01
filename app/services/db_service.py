from sqlalchemy.orm import Session
from app import models, database

def set_curfew(user_id: str, time_str: str):
    """門限を保存する"""
    with Session(database.engine) as session:
        curfew = session.query(models.Curfew).filter_by(user_id=user_id).first()
        if curfew:
            curfew.time = time_str
        else:
            curfew = models.Curfew(user_id=user_id, time=time_str)
            session.add(curfew)
        session.commit()

def get_curfew(user_id: str) -> str | None:
    """門限を取得する"""
    with Session(database.engine) as session:
        curfew = session.query(models.Curfew).filter_by(user_id=user_id).first()
        return curfew.time if curfew else None
