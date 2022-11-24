# Final Test

> [Code](./65010077.ino)

### Requirement

- กำหนดให้มีสวิตซ์จำนวน 4 ตัว และ LED จำนวน 4 ดวง
- ต้องต่อตัวต้านทานให้ถูกต้อง มิฉะนั้นจะไม่ตรวจ
- ให้สวิตซ์ 3 ตัว แทยตัวเลข Input กำหนดให้ กดหมายถึง 1 และ ไม่กด หมายถึง 0
- สวิตซ์ตัวที่ 4 ทำหน้าที่กดเพื่อรับข้อมูล
- เมื่อเริ่มทำงาน ให้ LED ทุกดวงดับหมด และรอรับข้อมูล 3 บิต ชุดที่ 1 (โดยการกดสวิตซ์ 3 ตัวแรก ค้างไว้และกดสวิตซ์ที่ 4 เพื่อรับ) จากนั้นให้นำผลของการป้อนข้อมูลมาแสดงผลที่ LED 3 ดวงขวาสุด
- จากนั้นรอรับข้อมูล 3 บิต ชุดที่ 2 (โดยการกดสวิตซ์ 3 ตัวแรก ค้างไว้และกดสวิตซ์ที่ 4 เพื่อรับ) จากนั้นให้นำผลของการป้อนข้อมูลมาแสดงผลที่ LED 3 ดวงขวาสุด
- เมื่อกดสวิตซ์ที่ 4 อีกครั้งให้นำเลขฐาน 2 ชุดที่ 1 มาทำการเติมบิต 0 เข้าไปข้างหน้า และทำการ Rotate แบบ Circular ไปทางซ้าย เป็นจำนวนเท่ากับจำนวนครั้งในข้อมูลชุดที่ 2 จากนั้นแสดงผลลัพธ์เป็นเลขฐาน 2 จำนวน 4 บิต ที่ LED ทั้ง 4 ดวง
- เมื่อกดสวิตซ์ตัวที่ 4 อีกครั้ง ให้เริ่มทำงานใหม่

### Example
```
ตัวเลขชุดที่ 1 = 101
ตัวเลขชุดที่ 12 = 011
เพิ่มบิต 0 เข้าไปหน้าตัวเลขชุดที่ 1 = 0101
ทำการ Rotate แบบ Circular จำนวน 3 ครั้ง
- ครั้งที่ 1 = 1010
- ครั้งที่ 2 = 0101
- ครั้งที่ 3 = 1010
จากนั้นให้นำตัวเลข 1010 มาแสดงที่ LED
```