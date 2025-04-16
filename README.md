While working on my philosophers project, I ran into several tricky issues with threads and synchronization. At first, managing shared data like lastmeal and iseating across multiple threads caused unexpected behavior and random deaths. I fixed these bugs through constant testing, digging into mutex usage, and reading up on thread safety. I also relied a lot on my peers for feedback and suggestions, which helped me see things I missed. Bit by bit, I patched the logic and now it works smoothly. Best part? It runs reliably on both Linux and macOS thanks to portable POSIX threads.



![Screenshot from 2025-04-16 20-34-11](https://github.com/user-attachments/assets/7c7788af-6d71-48c9-9eb8-469493148e23)
