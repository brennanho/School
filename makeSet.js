function makeSet(arr) {
	hashset = {};
	new_arr = [];
	for (let i = 0; i < arr.length; i++) {
		if (hashset[arr[i]] == undefined) {
			new_arr.push(arr[i]);
		}
		hashset[arr[i]] = arr[i];
	}
	return new_arr;
}


function makeSet2(arr) {
	hashset = {};
	i = 0;
	while (i < arr.length) {
		if (hashset[arr[i]] != undefined){
			arr.splice(arr[i--], 1);
		}	
		hashset[arr[i]] = arr[i++];
	}
	return arr;
}

console.log(makeSet2([1,2]));