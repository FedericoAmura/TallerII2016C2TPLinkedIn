#!/usr/bin/python
import unittest
print "Hello Tester!!!"

def fun(x):
    return x+1

class TestRunner(unittest.TestCase):
    #cada funcion es un test
    def prueba(self):
        self.assertEqual(fun(3), 4, "Test Prueba");