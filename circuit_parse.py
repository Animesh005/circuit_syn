path_to_netlist = input("Enter the path to input netlist: ")
path_to_netlist_out = input("Enter the path to output netlist: ")

# read Block-Cipher netlist
with open(path_to_netlist) as f:
    content_list = f.readlines()

# remove new line characters
content_list = [x.strip() for x in content_list]

result = []

for x in content_list:
    temp = x.split()
    # int_temp = [int(y) for y in temp]
    result.append(temp)

print("Netlist size before: ", len(result))

for sublist in result:
    if 'XOR' in sublist:
        sublist[-1] = 0
    elif 'AND' in sublist:
        sublist[-1] = 1
    elif 'OR' in sublist:
        sublist[-1] = 2
    elif 'AND-XOR' in sublist:
        sublist[-1] = 10
    elif 'OR-XOR' in sublist:
        sublist[-1] = 20
    else:
        pass

# Open the file in write mode
with open(path_to_netlist_out, 'w') as file:
    for index, sublist in enumerate(result):
        file.write(' '.join(map(str, sublist)))
        if index < len(result) - 1:
            file.write('\n')