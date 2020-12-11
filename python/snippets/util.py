def FileExists(filename):
    try:
        open(filename, "r")
        return True
    except IOError:
#         print("Error: File does not appear to exist.")
        return False