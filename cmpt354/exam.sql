1.
SELECT DISTINCT author_name, publisher_name
	FROM book, book_authors
	WHERE book.book_id = book_authors.book_id;

2.  ###
WITH (SELECT DISTINCT card_no, name, title
	FROM book LEFT OUTER JOIN book_loans
	USING (card_no)) AS cust_books
	SELECT card_no, name, title
		from cust_books LEFT OUTER JOIN
		book using (book_id)

