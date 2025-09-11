import os
from dotenv import load_dotenv

load_dotenv()

DATABASE_URL = os.getenv("DATABASE_URL", "sqlite:///./test.db")

CHANNEL_ACCESS_TOKEN = os.getenv("CHANNEL_ACCESS_TOKEN")
CHANNEL_SECRET = os.getenv("CHANNEL_SECRET")

ASSEMBLYAI_API_KEY = os.getenv('ASSEMBLYAI_API_KEY')

LINE_USER_ID = os.getenv('LINE_USER_ID')
