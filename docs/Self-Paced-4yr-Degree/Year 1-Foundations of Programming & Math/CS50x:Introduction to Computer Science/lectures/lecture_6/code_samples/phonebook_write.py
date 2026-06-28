# Writing file to csv

import csv

# Open file
file = open("phonebook.csv", "a")

# Get nane and number from user
name = input("Name: ")
number = input("Number: ")

# Write to csv file
writer = csv.writer(file)
writer.writerow([name, number])

# Close file
file.close()
