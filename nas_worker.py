import sys
sys.path.insert(1,'/export/scratch1/home/shared/nas_online')
import json

import nas_test
nas_test.init("cifar100")

arguments = sys.argv
encodings_fn = arguments[1]

with open(encodings_fn) as json_file:
    data = json.load(json_file)
    encodings = data["encodings"]
    print("Encodings: ", encodings)

for encoding in encodings:
    nas_test.evaluate(encoding)
