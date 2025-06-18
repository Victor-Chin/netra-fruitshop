import requests
import config

def process_payment(card_number, amount):
    response = requests.post("https://payment.example.com/api/pay", 
                             json={"card_number": card_number, "amount": amount},
                             headers={"Authorization": f"Bearer {config.PAYMENT_API_KEY}"})
    return response.json().get("transaction_id")
