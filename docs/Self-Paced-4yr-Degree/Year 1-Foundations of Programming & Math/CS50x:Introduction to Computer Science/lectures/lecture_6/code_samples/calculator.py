# Truncation and Floating Point Imprecision

# Prompt the user for x
x = int(input("x: "))

# Prompt the user for y
y = int(input("y: "))

# Divide x by y
z = x / y
print(f"{z:.50f}")
