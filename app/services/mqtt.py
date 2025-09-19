import ssl, json

from paho.mqtt import client as mqtt_client

from app.services.recognize_audio_service import append_csv
from app.config import MQTT_PASSWORD, MQTT_USERNAME, MQTT_PORT, MQTT_BROKER_ADDRESS, MQTT_CLIENT_ID


broker = MQTT_BROKER_ADDRESS
port = MQTT_PORT
topic = "python/mqtt"
client_id = MQTT_CLIENT_ID

topic_audio = "api/device/audio"

mqtt_client_instance: mqtt_client.Client | None = None

def on_connect(client, userdata, flags, rc):
    if rc == 0:
        print("[MQTT] Connected to MQTT Broker!")
        client.subscribe(topic_audio)
    else:
        print("[MQTT] Failed to connect, return code %d\n", rc)


def on_message(client, userdata, msg):
    payload = msg.payload.decode()
    print(f"[MQTT] {msg.topic}: {payload}")

    if msg.topic == topic_audio:
        # 音声データの受付
        try:
            data = json.loads(payload)  # ← JSONを辞書に変換
            samples = data.get("audio_data", [])
            if samples:
                append_csv(samples)
                print(f"[CSV] Appended {len(samples)} samples")
            else:
                print("[CSV] No audio_data in payload")
        except Exception as e:
            print(f"[ERROR] Failed to append audio: {e}")


def connect_mqtt() -> mqtt_client:
    client = mqtt_client.Client(client_id=client_id)

    client.username_pw_set(MQTT_USERNAME, MQTT_PASSWORD)
    client.tls_set(cert_reqs=ssl.CERT_NONE)
    client.tls_insecure_set(True)

    client.on_connect = on_connect
    client.on_message = on_message

    client.connect(broker, port)

    return client


# def subscribe(client: mqtt_client):
#     client.subscribe(topic)
#     client.on_message = on_message


def run_mqtt():
    global mqtt_client_instance
    mqtt_client_instance = connect_mqtt()
    mqtt_client_instance.loop_start()
    return mqtt_client_instance


def stop_mqtt():
    global mqtt_client_instance
    if mqtt_client_instance:
        mqtt_client_instance.loop_stop()
        mqtt_client_instance.disconnect()
        print("[MQTT] Client stopped")
        mqtt_client_instance = None
