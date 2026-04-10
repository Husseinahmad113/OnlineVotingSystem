#include <algorithm>
#include <iostream>
#include <limits>
#include <stack>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

struct Candidate {
    int id;
    string name;
    string party;
    int votes;
    Candidate* next;

    Candidate(int candidateId, const string& candidateName, const string& candidateParty)
        : id(candidateId), name(candidateName), party(candidateParty), votes(0), next(nullptr) {}
};

struct Voter {
    int id;
    string name;
    bool hasVoted;
    int votedCandidateId;

    Voter() : id(0), name(""), hasVoted(false), votedCandidateId(-1) {}

    Voter(int voterId, const string& voterName)
        : id(voterId), name(voterName), hasVoted(false), votedCandidateId(-1) {}
};

struct VoteAction {
    int voterId;
    int candidateId;
};

class OnlineVotingSystem {
private:
    unordered_map<int, Voter> voters;
    unordered_map<int, Candidate*> candidateIndex;
    Candidate* head;
    stack<VoteAction> voteHistory;

public:
    OnlineVotingSystem() : head(nullptr) {}

    ~OnlineVotingSystem() {
        Candidate* temp = head;
        while (temp != nullptr) {
            Candidate* nextNode = temp->next;
            delete temp;
            temp = nextNode;
        }
    }

    void clearInput() {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    Candidate* getCandidate(int id) {
        auto it = candidateIndex.find(id);
        if (it == candidateIndex.end()) {
            return nullptr;
        }
        return it->second;
    }

    void registerVoter() {
        int id;
        string name;

        cout << "Enter voter ID: ";
        cin >> id;

        if (cin.fail()) {
            clearInput();
            cout << "Invalid voter ID!\n";
            return;
        }
        clearInput();

        if (voters.find(id) != voters.end()) {
            cout << "Duplicate voter ID!\n";
            return;
        }

        cout << "Enter voter name: ";
        getline(cin, name);

        if (name.empty()) {
            cout << "Name cannot be empty!\n";
            return;
        }

        voters[id] = Voter(id, name);
        cout << "Voter registered successfully.\n";
    }

    void addCandidate() {
        int id;
        string name, party;

        cout << "Enter candidate ID: ";
        cin >> id;

        if (cin.fail()) {
            clearInput();
            cout << "Invalid candidate ID!\n";
            return;
        }
        clearInput();

        if (candidateIndex.find(id) != candidateIndex.end()) {
            cout << "Duplicate candidate ID!\n";
            return;
        }

        cout << "Enter candidate name: ";
        getline(cin, name);

        if (name.empty()) {
            cout << "Candidate name cannot be empty!\n";
            return;
        }

        cout << "Enter candidate party: ";
        getline(cin, party);

        if (party.empty()) {
            cout << "Party cannot be empty!\n";
            return;
        }

        Candidate* newNode = new Candidate(id, name, party);

        if (head == nullptr) {
            head = newNode;
        } else {
            Candidate* temp = head;
            while (temp->next != nullptr) {
                temp = temp->next;
            }
            temp->next = newNode;
        }

        candidateIndex[id] = newNode;
        cout << "Candidate added successfully.\n";
    }

    void displayCandidates() {
        if (head == nullptr) {
            cout << "No candidates available.\n";
            return;
        }

        Candidate* temp = head;
        while (temp != nullptr) {
            cout << "ID: " << temp->id
                 << " | Name: " << temp->name
                 << " | Party: " << temp->party
                 << " | Votes: " << temp->votes << "\n";
            temp = temp->next;
        }
    }

    void displayCandidatesSortedByVotes() {
        if (head == nullptr) {
            cout << "No candidates available.\n";
            return;
        }

        vector<Candidate*> candidateList;
        Candidate* temp = head;

        while (temp != nullptr) {
            candidateList.push_back(temp);
            temp = temp->next;
        }

        sort(candidateList.begin(), candidateList.end(),
             [](Candidate* a, Candidate* b) {
                 if (a->votes == b->votes) {
                     return a->name < b->name;
                 }
                 return a->votes > b->votes;
             });

        cout << "\nCandidates Sorted by Votes:\n";
        for (Candidate* c : candidateList) {
            cout << "ID: " << c->id
                 << " | Name: " << c->name
                 << " | Party: " << c->party
                 << " | Votes: " << c->votes << "\n";
        }
    }

    void searchVoter() {
        int id;

        cout << "Enter voter ID to search: ";
        cin >> id;

        if (cin.fail()) {
            clearInput();
            cout << "Invalid voter ID!\n";
            return;
        }

        auto it = voters.find(id);
        if (it == voters.end()) {
            cout << "Voter not found.\n";
            return;
        }

        cout << "Voter found:\n";
        cout << "ID: " << it->second.id
             << " | Name: " << it->second.name
             << " | Has Voted: " << (it->second.hasVoted ? "Yes" : "No") << "\n";
    }

    void searchCandidate() {
        int id;

        cout << "Enter candidate ID to search: ";
        cin >> id;

        if (cin.fail()) {
            clearInput();
            cout << "Invalid candidate ID!\n";
            return;
        }

        Candidate* c = getCandidate(id);
        if (c == nullptr) {
            cout << "Candidate not found.\n";
            return;
        }

        cout << "Candidate found:\n";
        cout << "ID: " << c->id
             << " | Name: " << c->name
             << " | Party: " << c->party
             << " | Votes: " << c->votes << "\n";
    }

    void updateVoter() {
        int id;
        string newName;

        cout << "Enter voter ID to update: ";
        cin >> id;

        if (cin.fail()) {
            clearInput();
            cout << "Invalid voter ID!\n";
            return;
        }
        clearInput();

        auto it = voters.find(id);
        if (it == voters.end()) {
            cout << "Voter not found.\n";
            return;
        }

        cout << "Enter new voter name: ";
        getline(cin, newName);

        if (newName.empty()) {
            cout << "Name cannot be empty!\n";
            return;
        }

        it->second.name = newName;
        cout << "Voter updated successfully.\n";
    }

    void updateCandidate() {
        int id;
        string newName, newParty;

        cout << "Enter candidate ID to update: ";
        cin >> id;

        if (cin.fail()) {
            clearInput();
            cout << "Invalid candidate ID!\n";
            return;
        }
        clearInput();

        Candidate* c = getCandidate(id);
        if (c == nullptr) {
            cout << "Candidate not found.\n";
            return;
        }

        cout << "Enter new candidate name: ";
        getline(cin, newName);

        cout << "Enter new candidate party: ";
        getline(cin, newParty);

        if (newName.empty() || newParty.empty()) {
            cout << "Name and party cannot be empty!\n";
            return;
        }

        c->name = newName;
        c->party = newParty;
        cout << "Candidate updated successfully.\n";
    }

    void deleteVoter() {
        int id;

        cout << "Enter voter ID to delete: ";
        cin >> id;

        if (cin.fail()) {
            clearInput();
            cout << "Invalid voter ID!\n";
            return;
        }

        auto it = voters.find(id);
        if (it == voters.end()) {
            cout << "Voter not found.\n";
            return;
        }

        if (it->second.hasVoted) {
            cout << "Cannot delete a voter who has already voted.\n";
            return;
        }

        voters.erase(it);
        cout << "Voter deleted successfully.\n";
    }

    void deleteCandidate() {
        int id;

        cout << "Enter candidate ID to delete: ";
        cin >> id;

        if (cin.fail()) {
            clearInput();
            cout << "Invalid candidate ID!\n";
            return;
        }

        Candidate* target = getCandidate(id);
        if (target == nullptr) {
            cout << "Candidate not found.\n";
            return;
        }

        if (target->votes > 0) {
            cout << "Cannot delete a candidate who already has votes.\n";
            return;
        }

        if (head == target) {
            head = head->next;
        } else {
            Candidate* prev = head;
            while (prev != nullptr && prev->next != target) {
                prev = prev->next;
            }
            if (prev != nullptr) {
                prev->next = target->next;
            }
        }

        candidateIndex.erase(id);
        delete target;

        cout << "Candidate deleted successfully.\n";
    }

    void castVote() {
        int voterId, candidateId;

        if (head == nullptr) {
            cout << "No candidates available.\n";
            return;
        }

        cout << "Enter voter ID: ";
        cin >> voterId;

        if (cin.fail()) {
            clearInput();
            cout << "Invalid voter ID!\n";
            return;
        }

        auto voterIt = voters.find(voterId);
        if (voterIt == voters.end()) {
            cout << "Invalid voter!\n";
            return;
        }

        if (voterIt->second.hasVoted) {
            cout << "Already voted!\n";
            return;
        }

        displayCandidates();

        cout << "Enter candidate ID: ";
        cin >> candidateId;

        if (cin.fail()) {
            clearInput();
            cout << "Invalid candidate ID!\n";
            return;
        }

        Candidate* c = getCandidate(candidateId);
        if (c == nullptr) {
            cout << "Invalid candidate!\n";
            return;
        }

        voterIt->second.hasVoted = true;
        voterIt->second.votedCandidateId = candidateId;
        c->votes++;

        voteHistory.push({voterId, candidateId});

        cout << "Vote cast successfully.\n";
    }

    void undoVote() {
        if (voteHistory.empty()) {
            cout << "Nothing to undo.\n";
            return;
        }

        VoteAction last = voteHistory.top();
        voteHistory.pop();

        voters[last.voterId].hasVoted = false;
        voters[last.voterId].votedCandidateId = -1;

        Candidate* c = getCandidate(last.candidateId);
        if (c != nullptr && c->votes > 0) {
            c->votes--;
        }

        cout << "Vote undone successfully.\n";
    }

    void results() {
        if (head == nullptr) {
            cout << "No candidates available.\n";
            return;
        }

        Candidate* temp = head;
        int highestVotes = -1;
        vector<Candidate*> winners;

        while (temp != nullptr) {
            cout << temp->name << " (" << temp->party << "): "
                 << temp->votes << " votes\n";

            if (temp->votes > highestVotes) {
                highestVotes = temp->votes;
                winners.clear();
                winners.push_back(temp);
            } else if (temp->votes == highestVotes) {
                winners.push_back(temp);
            }

            temp = temp->next;
        }

        if (highestVotes == 0) {
            cout << "No votes have been cast yet.\n";
        } else if (winners.size() > 1) {
            cout << "Tie detected between: ";
            for (size_t i = 0; i < winners.size(); i++) {
                cout << winners[i]->name;
                if (i < winners.size() - 1) {
                    cout << ", ";
                }
            }
            cout << "\n";
        } else {
            cout << "Current winner: " << winners[0]->name << "\n";
        }
    }

    void showTestCases() {
        cout << "\nSample test cases to demonstrate:\n";
        cout << "1. Register a voter successfully\n";
        cout << "2. Register duplicate voter ID\n";
        cout << "3. Add a candidate successfully\n";
        cout << "4. Add duplicate candidate ID\n";
        cout << "5. Search for existing voter/candidate\n";
        cout << "6. Update voter/candidate info\n";
        cout << "7. Cast a valid vote\n";
        cout << "8. Prevent duplicate voting\n";
        cout << "9. Undo last vote\n";
        cout << "10. Delete voter who has not voted\n";
        cout << "11. Prevent deleting candidate with votes\n";
        cout << "12. Display sorted results\n";
    }

    void menu() {
        int choice;

        do {
            cout << "\n===== Online Voting System =====\n";
            cout << "1. Register Voter\n";
            cout << "2. Add Candidate\n";
            cout << "3. Show Candidates\n";
            cout << "4. Cast Vote\n";
            cout << "5. Undo Vote\n";
            cout << "6. Show Results\n";
            cout << "7. Search Voter\n";
            cout << "8. Search Candidate\n";
            cout << "9. Update Voter\n";
            cout << "10. Update Candidate\n";
            cout << "11. Delete Voter\n";
            cout << "12. Delete Candidate\n";
            cout << "13. Show Candidates Sorted by Votes\n";
            cout << "14. Show Test Cases\n";
            cout << "0. Exit\n";
            cout << "Choice: ";
            cin >> choice;

            if (cin.fail()) {
                clearInput();
                cout << "Invalid choice. Try again.\n";
                continue;
            }

            switch (choice) {
                case 1:
                    registerVoter();
                    break;
                case 2:
                    addCandidate();
                    break;
                case 3:
                    displayCandidates();
                    break;
                case 4:
                    castVote();
                    break;
                case 5:
                    undoVote();
                    break;
                case 6:
                    results();
                    break;
                case 7:
                    searchVoter();
                    break;
                case 8:
                    searchCandidate();
                    break;
                case 9:
                    updateVoter();
                    break;
                case 10:
                    updateCandidate();
                    break;
                case 11:
                    deleteVoter();
                    break;
                case 12:
                    deleteCandidate();
                    break;
                case 13:
                    displayCandidatesSortedByVotes();
                    break;
                case 14:
                    showTestCases();
                    break;
                case 0:
                    cout << "Exiting system.\n";
                    break;
                default:
                    cout << "Invalid choice. Try again.\n";
            }
        } while (choice != 0);
    }
};

int main() {
    OnlineVotingSystem system;
    system.menu();
    return 0;
}