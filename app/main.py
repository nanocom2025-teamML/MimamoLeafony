from fastapi import Depends, FastAPI
from .database import Base, engine
from sqlalchemy.orm import Session
from app.database import get_db
from app.models import Target, TargetCreate
from app.routers import webhook

Base.metadata.create_all(bind=engine)

app = FastAPI()

app.include_router(webhook.router)

@app.get("/")
def read_root():
    return {"message": "Hello, MimamoLeafony!"}

@app.get("/targets/")
def read_targets(db: Session = Depends(get_db)):
    return db.query(Target).all()

@app.post("/targets/")
def create_target(target: TargetCreate, db: Session = Depends(get_db)):
    db_target = Target(**target.model_dump())
    db.add(db_target)
    db.commit()
    db.refresh(db_target)
    return db_target