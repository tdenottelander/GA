import sys
sys.path.insert(1,'/export/scratch1/home/shared/nas_online')

import nas_test
import json

with open('queue/encoding.json') as json_file:
    data = json.load(json_file)
    encoding = data["encoding"]
    print("Encoding: ", encoding)

nas_test.init("cifar100")
nas_test.evaluate(encoding)
