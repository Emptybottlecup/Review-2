#include <algorithm>
#include <iostream>
#include <string>
#include <sstream>
#include <string_view>
#include <vector>
#include <cassert>

using namespace std;

class Domain {
public:
    Domain(const string& line)
        : domain_(line.rbegin(), line.rend()) {
        domain_.push_back('.');
    }

    bool operator==(const Domain& other) const {
        return domain_ == other.domain_;
    }

    bool operator<(const Domain& other) const {
        return domain_ < other.GetDomain();
    }

    // разработайте метод IsSubdomain, принимающий другой домен и возвращающий true, если this его поддомен
    bool IsSubdomain(const Domain& other) const {

        if (domain_.size() < other.domain_.size()) {
            return false;
        }

        auto res = std::mismatch(other.domain_.begin(), other.domain_.end(), domain_.begin());

        return res.first == other.domain_.end();
    }

    const string& GetDomain() const {
        return domain_;
    }

private:
    string domain_;
};

class DomainChecker {
public:
    template<typename Iterator>
    DomainChecker(Iterator first, Iterator last)
        : forbidden_domains_(first, last) {
        sort(forbidden_domains_.begin(), forbidden_domains_.end());
        const auto it = std::unique(forbidden_domains_.begin(), forbidden_domains_.end(), [](const Domain& lhs, const Domain& rhs) {
            return rhs.IsSubdomain(lhs);
            });
        forbidden_domains_.erase(it, forbidden_domains_.end());
    }

    bool IsForbidden(const Domain& domain) const {
        auto it = upper_bound(forbidden_domains_.begin(), forbidden_domains_.end(), domain);
        if (it == forbidden_domains_.begin()) {
            return false;
        }
        else {
            return domain.IsSubdomain(*prev(it));
        }
    }

private:
    vector<Domain> forbidden_domains_;
};

// разработайте функцию ReadDomains, читающую заданное количество доменов из стандартного входа
template <typename Number>
vector<Domain> ReadDomains(istream& in, Number num) {
    vector<Domain> domains;
    domains.reserve(num);
    string domain;
    for (Number i = 0; i < num; ++i) {
        getline(in, domain);
        domains.push_back(domain);
    }
    return domains;
}

template <typename Number>
Number ReadNumberOnLine(istream& input) {
    string line;
    getline(input, line);

    Number num;
    std::istringstream(line) >> num;

    return num;
}

void TestDomain() {
    Domain check("gdz.ru");
    Domain check2("scum.ru");
    assert(check == Domain{ "gdz.ru" });
    assert(!(check == check2));
    assert(!(check.GetDomain() == check2.GetDomain()));
    assert(Domain{ "www.scum.ru" }.IsSubdomain(check2));
    assert(!(Domain{ "www.scum.ru" }.IsSubdomain(check)));
    assert(check2 < Domain{ "www.scum.ru" });
}

void TestDomainChecker() {

    vector<Domain> forbidden_domains{
        {"gdz.ru"},
        {"maps.me"},
        {"m.gdz.ru"},
        {"com"}
    };

    vector<Domain> test_true{
        {"alg.m.gdz.ue"},
        {"maps.ru"},
        {"gdz.ua"}
    };

    vector<Domain> test_false{
       {"gdz.ru"},
       {"gdz.com"},
       {"m.maps.me"},
       {"alg.m.gdz.ru"},
       {"maps.com"}
    };

    DomainChecker check(forbidden_domains.begin(), forbidden_domains.end());

    for (auto domain : test_true) {
        assert(check.IsForbidden(domain) == false);
    }

    for (auto domain : test_false) {
        assert(check.IsForbidden(domain) == true);
    }

}

void Test() {
    TestDomain();
    TestDomainChecker();
}

int main() {
    Test();
    const std::vector<Domain> forbidden_domains = ReadDomains(cin, ReadNumberOnLine<size_t>(cin));
    DomainChecker checker(forbidden_domains.begin(), forbidden_domains.end());

    const std::vector<Domain> test_domains = ReadDomains(cin, ReadNumberOnLine<size_t>(cin));
    for (const Domain& domain : test_domains) {
        cout << (checker.IsForbidden(domain) ? "Bad" : "Good") << endl;
    }
}