from flask import Flask, request, jsonify
from payment_gateway import process_payment
from fraud_detection import detect_fraud
from database import save_transaction
import config

app = Flask(__name__)

@app.route('/pay', methods=['POST'])
def pay():
    data = request.json
    if detect_fraud(data):
        return jsonify({"error": "Fraud detected"}), 403
    
    transaction_id = process_payment(data['card_number'], data['amount'])
    save_transaction(transaction_id, data)
    return jsonify({"transaction_id": transaction_id, "status": "Success"})

if __name__ == '__main__':
    app.run(debug=True)
