(define singleton?
	(lambda (x)
		(cond ((null? (cdr x)) #t)
				(else #f)
		)
	)
)

(define make-my-list
	(lambda (n x)
		(cond
			(( = n 0) '())
			(else 
				(cons x (make-my-list (- n 1) x)))
		)
	)	
)