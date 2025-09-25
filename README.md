# CÂY TIỀN TỐ TRIE

## Mô tả dự án
Đây là dự án xây dựng ứng dụng từ điển sử dụng cây tiền tố (TRIE) để giải quyết bài toán autocomplete (gợi ý từ). Dự án đồng thời so sánh hiệu quả giữa hai cấu trúc dữ liệu TRIE và TST (Ternary Search Tree) trong việc tìm kiếm và gợi ý từ.

## Video demo


## Tính năng chính
- Xây dựng và thao tác với cây TRIE và TST.
- Ứng dụng giao diện Windows (Win32 API) cho TRIE: nhập, tìm kiếm, thêm, xóa từ, gợi ý từ theo tiền tố.
- Đánh giá hiệu năng (thời gian, số phép so sánh) giữa TRIE và TST trên các bộ dữ liệu kiểm thử.


## Cấu trúc thư mục
```
data/
  words.txt          
  prefixTest1.txt   
  prefixTest2.txt    
  prefixTest3.txt     
source/
  TRIE/              
    TRIE/            
    TRIE.sln          
    TST/              
Report.pdf           
README.md             
```

## Hướng dẫn cài đặt & chạy
- **Yêu cầu:** Windows, Visual Studio 2022, C++
- **TRIE:** Mở solution `TRIE.sln` trong Visual Studio, chọn project TRIE, cấu hình dạng Application (.exe), build và chạy (Win32 GUI).
- **TST:** Mở project TST, build và chạy dạng Console Application.
- Dữ liệu từ điển nằm trong thư mục `data/` (file `words.txt`, các file test tiền tố).


## Một số file chính
- `Trie.cpp`, `trie.h`: Cài đặt cấu trúc và thuật toán TRIE
- `main.cpp`: Giao diện và xử lý sự kiện ứng dụng TRIE
- `TST/main.cpp`: Cài đặt và kiểm thử TST
- `Report.pdf`: Báo cáo tổng kết



