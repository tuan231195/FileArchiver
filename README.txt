1. Utils.h: define auxiliary functions for the program
	- blockHashing(char *, length, int *saved): hash a block (pass NULL to block if we  just want to compute the hash of an individual block)
2. Database class:
	- Connection * getConnection(): 
		+ returns the connection to the database
		+ throw SQLException if MySQL is turned off 
3. FileRecord class:
	a)Member functions:
	- FileRecord(string filename)
	- getters and setters
	- Note: no setters for comments, versionids, and blockhashes. It is the 		responsibility of the FileArchiver to transfer comments and versionids to the 		database (since they will be updated a lot throughout the program). FileRecord 		class only gets those data from the database. saveToDatabase function will handle 	block hashes (only call it when create a new file record or update the file 		reference version since it will update the fileblkhashes table as well as the 		whole file record)
	- createData(): 
		+ create the data for the file record object by reading the corresponding 		file on the local file system, which can be saved to the database or 			discarded later
		+ throw runtime_error if the file does not exist
	- saveToDatabase():
		+ if the file name does not exist, insert a new file record into the 			database.
		+ if it does, update the file record and the blockhashes table (should 			only be used when update the reference version)
		+ throw SQLException
	- bool getDataFromDatabase():
		+ if the file record does not exist, return false
		+ else, update the data in the file record
		+ throw SQLException
	b)Instance variables:
	- Database database: used to connect to the persistent storage
	- string fileName
	- string tempName: location of the compressed file, used as a foreign key to 		retrieve the reference version from the database
	- timespec modifyTime
	- long fileLength
	- int nVersions: total number of versions
	- int curHash: the current hash of the file
	- int ovHash: the hash of the reference version of the file 
	- vector comments
	- vector blockHashes: the hash of all the blocks in the latest version
4. VersionRec class:
	- VersionRec(string fileRef): used when we want to create a new version record
	- VersionRec(int id, string fileRef): used when we want to retrieve a particular 	version record from the database
	- getters and setters
	- getDataFromDatabase: get data from the database
	- saveDataToDatabase: insert a new version record to the database and update block 	table 