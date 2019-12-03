print('hello module')

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

l = [5, 6, 7];
i = 8;
print(listAppend(l, i))
