import sys

result = 0

with open(sys.argv[1], 'r') as file:
    for line in file:
        try:
            result = (result + int(line)) % 256
        except ValueError:
            continue


with open(sys.argv[2], 'w') as file:
    file.write(str(result))