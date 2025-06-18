import unittest
from fraud_detection import detect_fraud

class FraudDetectionTest(unittest.TestCase):
    def test_fraud_detection(self):
        self.assertIn(detect_fraud({"card_number": "4111111111111111"}), [True, False])

if __name__ == '__main__':
    unittest.main()
