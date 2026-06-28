# Exits with explicit value, importing sys

import sys

# Exit if command-line argument is not 2
if len(sys.argv) != 2:
    print("Missing command-line argument")
    sys.exit(1)
else:
    print(f"hello, {sys.argv[1]}")
    sys.exit(0)

