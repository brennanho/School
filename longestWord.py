def longestWord(s):
	longest_word = ""
	curr_word = ""
	for char in s:
		if char.isalpha():
			curr_word += char
		else:
			curr_word = ""

		if len(curr_word) > len(longest_word):
			longest_word = curr_word

	return longest_word


print(longestWord("hello how are you doing?"))