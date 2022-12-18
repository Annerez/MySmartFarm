import firebase from 'firebase/app';
import 'firebase/database';
import 'firebase/analytics';
import { useState, useEffect } from 'react';

export default function app() {
    const firebaseConfig = {
        apiKey: "AIzaSyBhI5oYvb9WVDxGrWFRuKzHqhjz8D7UknI",
        authDomain: "smartfarm-23892.firebaseapp.com",
        databaseURL: "https://smartfarm-23892-default-rtdb.asia-southeast1.firebasedatabase.app",
        projectId: "smartfarm-23892",
        storageBucket: "smartfarm-23892.appspot.com",
        messagingSenderId: "145364417741",
        appId: "1:145364417741:web:bf3de6c33c1e7660dfbd84",
        measurementId: "G-L6GT79LLN6"
    };
    firebase.initializeApp(firebaseConfig);
    const analytics = firebase.analytics();
    const db = firebase.database();

    const TempRef = db.ref('SmartFarm/Temperature');
    const HumRef = db.ref('SmartFarm/Humidity');
    const HeatRef = db.ref('SmartFarm/HeatIndex');
    const LightRef = db.ref('SmartFarm/Light');
    const SoilRef = db.ref('SmartFarm/Soil');

    function MyComponent() {
        const [temp, setTemp] = useState(null);
        const [hum, setHum] = useState(null);
        const [heat, setHeat] = useState(null);
        const [light, setLight] = useState(null);
        const [soil, setSoil] = useState(null);

        useEffect(() => {
            TempRef.on('value', snapshot => {
                const data = snapshot.val();
                setTemp(data);
            });
            HumRef.on('value', snapshot => {
                const data1 = snapshot.val();
                setHum(data1);
            });
            HeatRef.on('value', snapshot => {
                const data2 = snapshot.val();
                setHeat(data2);
            });
            LightRef.on('value', snapshot => {
                const data3 = snapshot.val();
                setLight(data3);
            });
            SoilRef.on('value', snapshot => {
                const data4 = snapshot.val
                setSoil(data4);
            });
        });
    }

}