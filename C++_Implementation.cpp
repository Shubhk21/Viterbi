#include "bits/stdc++.h" 
using namespace std; 
#define mod 1e9 + 7 
#define FOR(a, c) for (int(a) = 0; (a) < (c); (a)++) 
#define FORL(a, b, c) for (int(a) = (b); (a) <= (c); (a)++) 
#define FORR(a, b, c) for (int(a) = (b); (a) >= (c); (a)--) 
#define INF 1000000000000000003 
typedef long long int ll; 
typedef vector<int> vi; 
typedef pair<int, int> pi; 
#define F first 
#define S second 
#define PB push_back 
#define POB pop_back 
#define MP make_pair 

// returns words in given sentence
vector<string> findwords(string sent){

    vector<string> words;

    
    int prevloc=0;

    for(int i=0;i<sent.length();i++){

        if(sent[i]==' '){
            words.push_back({sent.substr(prevloc,i-prevloc)});
            prevloc=i+1;
        }

    }
    words.push_back(sent.substr(prevloc,sent.length()-prevloc));
    return words;

}

int main() 
{ 
	ios::sync_with_stdio(0); 
    cin.tie(0); 
    
    string sentence = "Luxury cars undermine market patterns";

    vector<string> possibletags{"JJ", "NNS" , "VB" , "NN"};


    // Preprocessing steps: Storing emission and transition matrix in hash table.
    // This we assume students will do using provided matrices in excel format using parsing (Ex: pandas in python).

    map<pair<string,string>,double> emission;   // emission matrix (emission[{x,y}] gives probability of word 'x' being 'y' POS).

    map<pair<string,string>,double> transition;  // transition matrix (transition[{x,y}] gives probability of POS 'y' given it's previous POS is 'x').

    emission[{"Luxury","JJ"}]=0.00001634531841;
    emission[{"Luxury","NNS"}]=0.00000001670012899;
    emission[{"Luxury","VB"}]=0.00000003779035701;
    emission[{"Luxury","NN"}]=0.0003459794159;

    emission[{"cars","JJ"}]=0.00000001632898942;
    emission[{"cars","NNS"}]=0.002304634501;
    emission[{"cars","VB"}]=0.00000003779035701;
    emission[{"cars","NN"}]=0.000000007521128146;

    emission[{"undermine","JJ"}]=0.00000001632898942;
    emission[{"undermine","NNS"}]=0.00000001670012899;
    emission[{"undermine","VB"}]=0.0002645702894;
    emission[{"undermine","NN"}]=0.000000007521128146;

    emission[{"market","JJ"}]=0.00000001632898942;
    emission[{"market","NNS"}]=0.00000001670012899;
    emission[{"market","VB"}]=0.0008314256446;
    emission[{"market","NN"}]=0.01395922136;

    emission[{"patterns","JJ"}]=0.00000001632898942;
    emission[{"patterns","NNS"}]=0.0001837181191;
    emission[{"patterns","VB"}]=0.00000003779035701;
    emission[{"patterns","NN"}]=0.000000007521128146;

    transition[{"JJ","JJ"}]=0.07466876138;
    transition[{"JJ","NNS"}]=0.2321085699;
    transition[{"JJ","VB"}]=0.0001143635712;
    transition[{"JJ","NN"}]=0.4500380662;


    transition[{"NNS","JJ"}]=0.01679030052;
    transition[{"NNS","NNS"}]=0.01052526924;
    transition[{"NNS","VB"}]=0.00390939622;
    transition[{"NNS","NN"}]=0.02081662728;

    transition[{"VB","JJ"}]=0.08465077185;
    transition[{"VB","NNS"}]=0.05072239454;
    transition[{"VB","VB"}]=0.004917193956;
    transition[{"VB","NN"}]=0.06161578658;


    transition[{"NN","JJ"}]=0.008786253401;
    transition[{"NN","NNS"}]=0.07779740039;
    transition[{"NN","VB"}]=0.001399187089;
    transition[{"NN","NN"}]=0.1221724556;

    transition[{"START","JJ"}]=0.04195117168;
    transition[{"START","NNS"}]=0.04190096085;
    transition[{"START","VB"}]=0.0028871477;
    transition[{"START","NN"}]=0.04011847646;



    vector<string> words = findwords(sentence);  // fetch all the words from sentence


    double dp[possibletags.size()][words.size()];  // dp table

    int tracker[possibletags.size()][words.size()]; // backtrack table stores index of previous column which gave us maximun probability at each cell.

    for(int j= 0; j<words.size();j++){

        for(int i=0;i<possibletags.size();i++){

            if(j==0){
                // initialization of DP table as this is first words (START---->THE).
                dp[i][j] = emission[{words[j],possibletags[i]}] * transition[{"START",possibletags[i]}];
                // we dont have to backtrack from first word so marked as -1.
                tracker[i][j]=-1;
            }

            else{

                double maxprobability=INT_MIN;
                int previousmaxtracker=-1;
                for(int k =0;k<possibletags.size();k++){

                    if(emission[{words[j],possibletags[i]}]*transition[{possibletags[k],possibletags[i]}]*dp[k][j-1]>maxprobability){
                        maxprobability = emission[{words[j],possibletags[i]}]*transition[{possibletags[k],possibletags[i]}]*dp[k][j-1];
                        previousmaxtracker= k;
                    }

                }
                // insert maximum probability we got.
                dp[i][j] = maxprobability;
                // insert the index of previous tag which gave us maximum probability
                tracker[i][j] = previousmaxtracker;

            }


        }

    }


    // Lets print the DP table
    cout<<"DP Table:\n";
    for(int i=0;i<possibletags.size();i++){
        for(int j=0;j<words.size();j++){
 
            cout<<dp[i][j]<<"               ";

        }
        cout<<endl;
    }


    // Lets print the tracker table
    cout<<endl;
    cout<<"Tracker Table:\n";
    for(int i=0;i<possibletags.size();i++){
        for(int j=0;j<words.size();j++){
 
            cout<<tracker[i][j]<<"           ";

        }
        cout<<endl;
    }


    // Find the maximum probability from last column so that we backtrack from that cell.
    int lastbeststate;
    double lasthighprobability=0;
    for(int i=0;i<possibletags.size();i++){

        if(dp[i][words.size()-1]>lasthighprobability){
            lasthighprobability=dp[i][words.size()-1];
            lastbeststate=i;
        }

    }

    vector<int> backtrack;

    int width = words.size();

    // We backtrack from that cell till the first word.
    while(lastbeststate!=-1){

        backtrack.push_back(lastbeststate);

        lastbeststate = tracker[lastbeststate][--width];

    }
    cout<<endl;

    reverse(backtrack.begin(),backtrack.end());


    // Print the POS tags from start to end for each word.
    cout<<"POS Tags:\n";
    for(auto tags: backtrack){

        cout<< possibletags[tags]<<"--->";

    }
    cout<<"END";

    return 0;
} 
