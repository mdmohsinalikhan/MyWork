Term Project 2013: Election outcome prediction

Introduction
The topic of the term project in 2013 is election outcome prediction. For deadlines, see the course calendar.


Obligatory parts:
	i)  Solve the given two problems
	ii) Report results

Voluntary parts:
	i)  Prediction challenge


The two problems:
	Predict the following class labels of political election candidates using their political views and background information as input data:
		i)  Election outcome of the candidate (elected/not elected)
		ii) Political party of the candidate (one-of-many)

Data set:
	i)  Rows correspond to candidates (=samples)
	ii) Columns:
		a) First two columns are the class labels
		b) Remaining columns are answers to questions (=variables)

Training set:
	Contains both the input data and the corresponding class labels
	Use for training your model
	You need to use this for initial model selection, e.g. for running cross-validation.
	File: See Material below

Test set:
	Don't use for training your model
	Contains only the input data until the Prediction Challenge
	Labels will be released after the Prediction Challenge
	File: See Material below

Code for reading the data into Matlab and R
	Files: See Material below

Details (not necessary to know to complete the task)

Meaning of the values in the data:
	Multi-choice question columns:
		The smallest value in a column corresponds to the answer "No."
		Larger values correspond to the answer "Yes," weighted by the importance of the question given by the candidate (the larger, the more 				important).
	Comment columns (=columns with a name containing the word "kommentti" and few others):
		Value encodes the length of the textual answer, weighted by the importance in a similar way as above.
		The smallest value in a column corresponds to a missing comment.


Pre-processing:
	Data sets have been pre-processed identically based on the training data.
	Each column of the training data has a mean of zero and standard deviation of one.
	This is why the data does not consist of integer values.
	Many methods work more reliably when the data is pre-processed in this way.


Methods
	All machine learning methods accepted, including methods outside the course material.
	Different method may be used for each problem.
	Any programming language/software accepted, as long as the deliverables are in the required format.
	Use suitable standard evaluation criteria, such as the F-score, which is used in the Prediction Challenge as an evaluation metric



Final report
	Obligatory
	Can be done in pairs
	Follow the formalized structure, see Lecture 2: Report writing.
	Don't forget to report your model's classification performance on the test data
	You can use F-score as one of the measures of performance
	Use figures to support your reasoning, and discuss every figure
	If you work as a pair, submit only one report
	Include name(s), student number(s) and the text "T-61.3050 Term Project, final report" under the title
	Submit your report as a single pdf-file to the course email address, t613050@ics.aalto.fi
	Filenames
		one person: final-report-12345X.pdf
		two persons: final-report-12345X-23456Y.pdf
		where 12345X and 23456Y are replaced by your student number(s).
		The prediction problem is not easy - Good luck!

Disclaimer:

We reserve the right to make changes to deadlines, data sets etc., given a good reason.

Responsible Assistants
Tuomas Rajala
Tommi Suvitaival
