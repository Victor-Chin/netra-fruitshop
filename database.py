import mysql.connector
import config

def get_db_connection():
    return mysql.connector.connect(
        user=config.DB_CREDENTIALS['user'],
        password=config.DB_CREDENTIALS['password'],
        host=config.DB_CREDENTIALS['host'],
        database=config.DB_CREDENTIALS['db']
    )

def save_transaction(transaction_id, data):
    conn = get_db_connection()
    cursor = conn.cursor()
    cursor.execute("INSERT INTO transactions (id, amount, card_number) VALUES (%s, %s, %s)", 
                   (transaction_id, data['amount'], data['card_number']))
    conn.commit()
    cursor.close()
    conn.close()

