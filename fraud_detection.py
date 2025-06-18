import hashlib

def detect_fraud(transaction_data):
    risk_score = hashlib.md5(transaction_data['card_number'].encode()).hexdigest()[:3]  # Proprietary risk score logic
    return int(risk_score, 16) % 2 == 0  # Mock fraud detection
