from fastapi import APIRouter, Depends
from sqlalchemy.orm import Session
from app.database import get_db
from app.models import Message, MessageCreate


router = APIRouter()


@router.get("/messages")
def read_messages(db: Session = Depends(get_db)):
    return db.query(Message).all()


@router.post("/messages")
def create_message(message: MessageCreate, db: Session = Depends(get_db)):
    db_message = Message(**message.dict())
    db.add(db_message)
    db.commit()
    db.refresh(db_message)
    return db_message
