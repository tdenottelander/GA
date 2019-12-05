myInteger = 5

def foo ():
    print("foo")

def bar ():
    return 2 + 4

def foobar (x):
    return 2 + x

def add (x, y):
    return x + y

def listAppend(list, integer):
    list.append(integer)
    return list

def incrementInteger():
    global myInteger
    myInteger += 1
    print("integer value", myInteger)

print("Load python_functions")
