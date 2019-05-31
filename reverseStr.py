def reverseStr(s):
	if len(s) == 0:
		return ""
	elif len(s) == 1:
		return s[0]
	else:
		return s[-1] + reverseStr(s[:len(s)-1])

def reverseStr2(s):
	s2 = ""
	end = len(s) - 1
	for i in range(len(s)):
		s2 += s[end-i]

	return s2

print(reverseStr2("hello"))