# A program that calculates the reading level of some text

def main():
    # Prompt the user for some text
    text = input("Text: ")

    # Count the number of letters, words, and sentences in the text
    letters = count_letters(text)
    words = count_words(text)
    sentences = count_sentences(text)

    # Compute the Coleman-Liau index
    index = compute_index(letters, words, sentences)

    # Print the grade level
    if index < 1:
        print("Before Grade 1")
    elif index >= 16:
        print("Grade 16+")
    else:
        print(f"Grade {index}")

    # Exit sucessfully
    return 0

# Count numbers of letters in text
def count_letters(text):
    # Keep track of letters
    letters = 0

    # Count letters in text
    for i in range(len(text)):
        if text[i].isalpha():
            letters += 1

    # Return letter count
    return letters

# Count numbers of words in text
def count_words(text):
    # Keep track of words
    words = 0

    # Count words in a string
    for i in range(len(text)):
        if text[i] == ' ':
            words += 1
    words += 1

    # Return words of words
    return words

# Count number of sentences in text
def count_sentences(text):
    # Keep track of sentences in string
    sentences = 0

    # Count sentences in text
    for i in range(len(text)):
        if text[i] == '.' or text[i] == '!' or text[i] == '?':
            sentences += 1

    # Return count of sentences
    return sentences

# Compute the Coleman-Liau index
def compute_index(letters, words, sentences):
    # Calculate the values of L and S
    L = float(letters) / float(words) * 100

    S = float(sentences) / float(words) * 100

    # Compute Coleman-Liau index
    index = (0.0588 * L) - (0.296 * S) - 15.0

    # Return rounded index
    return int(round(index))

# Call main
main()
