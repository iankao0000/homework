#!/usr/bin/python

import sys

# print(sys.argv[1])

result = []
with open(sys.argv[1], 'r') as f:
    tmp = f.readline()
    while (tmp):
        result.append(tmp)
        tmp = f.readline()
    f.close()
#   print(result)

result_split = []
for line in result:
    result_split.extend(line.split())
#   print(result_split)

total = int(result_split[-1]) + 1
#   print("total = %d" %total)

for i in range(0, total):
    #   print('i =', i, ', result_split[', i, '] =', result_split[i], '\n')
    if(int(i) != int(result_split[i])):
        print('%s failed\n' % str(i))
        print('output: %s' % str(result_split[i]))
        print('expected: %s' % str(i))
        exit()
    
print("for file " + sys.argv[1] + ", everything is correct!")
