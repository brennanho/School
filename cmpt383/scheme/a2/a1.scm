;q1
(define singleton?
	(lambda (x)
		(cond ((null? (cdr x)) #t)
				(else #f)
		)
	)
)

;q2
(define my-make-list
	(lambda (n x)
		(cond
			(( = n 0) '())
			(else 
				(cons x (my-make-list (- n 1) x))
			)
		)
	)	
)

;q3 NOT DONE
(define my-iota 
	(lambda (n)
		(cond
			((= n 0) '())
			(else
				(cons (- n n) (my-iota (- n 1)))
			)
		)
	)
)

;Q4
(define my-len
	(lambda (lst)
		(cond
			((null? lst) 0)
			(else
				(+ 1 (my-len (cdr lst)))
			)
		)
	)
)

;q5
(define nth
	(lambda (lst i)
		(cond 
			((null? lst) (error "bad index"))
			((= i 0) (car lst))
			(else
				(nth (cdr lst) (- i 1))
			)
		)
	)
)

;q6
(define my-last 
	(lambda (lst)
		(cond
			((null? lst) (error "my-last: empty list"))
			((= (my-len lst) 1) (car lst))
			(else
				(my-last (cdr lst))
			)
		)
	)
)

;q7
(define my-filter
	(lambda(pred lst)
		(cond
			((null? lst) '())
			((pred (car lst)) (my-filter pred (car lst)))
			(else
				(my-filter pred (cdr lst))
			)
		)
	)
)
