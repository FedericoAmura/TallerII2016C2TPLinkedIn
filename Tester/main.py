#!/usr/bin/python
import unittest
print "Hello Tester!!!"

def fun(x):
    return x+1

class MyTest(unittest.TestCase):
    def test(self):
        self.assertEqual(fun(3), 4, "Test Prueba");