function reverseStr(s) {
	let revs = "";
	for (let i = 0; i < s.length; i++) {
		revs = revs + s[s.length-i-1];
	}
	return revs;
}

console.log(reverseStr("hello"));