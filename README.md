# MultClientRep

This code is a multiple clients-server implementation of the K-Nearest Neighbors (KNN) classification algorithm. 

We created a server thats gets a port as argument. The server will run infinitely and for every new client that connects it, it will open a new thread with its own CLI - command line interface. That way, the sever can run multiple clients at once, every one in its own thread. Every client will gets an ip and a port as arguments.
The CLI will print the client this menu:

  "Welcome to the KNN Classifier Server. Please choose an option:
  1. upload an unclassified csv data file
  2. algorithm settings
  3. classify data
  4. display results
  5. download results
  8. exit"
  
 For every option in the menu, the CLI has a command that does the necessary actions for that option. All the commands are stored in a vector of commands in the CLI.
 The client will ask the user to choose an option and will send it to the CLI. According to the chosen option, the CLI will execute the right command. During the execute, the CLI and the client communicates by socketIO function, and the client gets/prints information to the user by cin/cout functions.  
 
 Explanation of the options in the menu:
 1. upload an unclassified csv data file - the client will get from the user a train file and a test file (that has to be CSV files) and will send it to the CLI for upload. The CLI will check that the files are valid and can be open. If the files are valid - the client will print "Upload complete.". If they are not valid it will print "invalid input".
 2. algorithm settings - the client will print to the user the current k and distance metric (every client start with k=5, distance metric="AUC"). Then the client will have the option to press enter, leave the settings as they are and go back to the menu, or, he could enter new k and new distance metric. If they are valid, the CLI will update them to be the new algorithm settings.
 3. classify data - the CLI will ren the KNN classification algorithm. If everything works as it should, the client will prints "classification data complete". If the user didn't upload files before, the client will print "please upload data".
 4. display results - the client will print to the user a list of clssifications. The list will be a list of couples - the number of the line in the text file, and the clssification. At the end of the list it will print "Done.".
 5. download results - the client gets a path from the user. Then, the client will open a new thread and in that thread it will write the list of clssifications to that file, without the "Done." in the end. The new thread allows the client to continue working with the CLI, and getting options from the user, while the data is still writing to the file.
 8. exit - all the information of the specific client will delete and the connection between this client and the server will end. 
All the data that the client prints to the user it gets from the CLI. All the data that the CLI needs for the actions he executes, the client gets from the user and sends it to the CLI.

You can see more information about the KNN algorithm and about the server-client connection in our previous exercises. 

For executing the code we need to run the following command: make

The server will be activated by: server.out file port

The client will be activated by: client.out ip port

Then, the menu will be printed and the client will get options from the user and work with the CLI to execute them.
