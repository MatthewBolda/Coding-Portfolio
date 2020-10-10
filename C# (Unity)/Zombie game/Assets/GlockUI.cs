using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class GlockUI : MonoBehaviour
{
    public Text ammoText;
    public Text nameText;
    public Glock gun;
    // Update is called once per frame
    void Start()
    {
        nameText.text = gun.name;
    }
    void Update()
    {
        string ammo = gun.currentAmmo.ToString() + "/" + gun.maxAmmo.ToString();
        ammoText.text = ammo;
    }
}
