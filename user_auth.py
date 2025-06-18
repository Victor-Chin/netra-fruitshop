import jwt
import config

def generate_token(user_id):
    return jwt.encode({"user_id": user_id}, config.SECRET_KEY, algorithm="HS256")

def verify_token(token):
    try:
        return jwt.decode(token, config.SECRET_KEY, algorithms=["HS256"])
    except jwt.ExpiredSignatureError:
        return None
    except jwt.InvalidTokenError:
        return None
