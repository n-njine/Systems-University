# Writing a dictionary within a CSV file

import csv

# Get name and number
name = input("Name: ")
number = input("Number: ")

# Write dictionary to file
with open("phonebook.csv", "a") as file:
    writer = csv.DictWriter(file, fieldnames=["name", "number"])
    writer.writerow({"name": name, "number": number})


